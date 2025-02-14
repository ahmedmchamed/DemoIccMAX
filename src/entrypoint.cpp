#include <cstdint>
#include <vector>

#include "ColourData.h"
#include "CommandLineUtility.h"
#include "IccCmm.h"
#include "IccProfile.h"
#include "icProfileHeader.h"

int main(int argc, char *argv[]) {
    std::vector<std::string> commandLineArgs{};

    // skip the program name itself by indexing from 1
    for (std::uint32_t i{1}; i < argc; ++i) {
        commandLineArgs.emplace_back(argv[i]);
    }

    IccConvert::CommandLineUtility utility{};
    IccConvert::ColourData const colourData{utility.parseCommandLineArgs(commandLineArgs)};

    CIccProfile *profile = ReadIccProfile(colourData.getProfile().c_str());

    if (!profile) {
        return icCmmStatCantOpenProfile;
    }

    if (profile->m_Header.deviceClass != icSigInputClass &&
        profile->m_Header.deviceClass != icSigDisplayClass &&
        profile->m_Header.deviceClass != icSigOutputClass &&
        profile->m_Header.deviceClass != icSigColorSpaceClass) {
        return icCmmStatInvalidProfile;
    }

    const auto intent = static_cast<icRenderingIntent>(static_cast<std::uint32_t>(colourData.getRenderIntent()));
    const auto sourceSpace = colourData.isDeviceToPcs() ? profile->m_Header.colorSpace : profile->m_Header.pcs;
    const auto destinationSpace = colourData.isDeviceToPcs() ? profile->m_Header.pcs : profile->m_Header.colorSpace;
    CIccCmm profileApplier(sourceSpace, destinationSpace, colourData.isDeviceToPcs());

    // unclear why this is a choice, and set to false in ICC roundtrip demo ("useMPE" - multiProcessElement)
    // spec says DToB takes precedence over all other tags...
    // ... "except where this tag is not needed or supported by the CMM"
    // does that mean it's valid to choose not to implement DToB conversion and simply not support it? 🤷
    constexpr auto useDToBTags = false;

    // additionally, who determines this? LutColor might be more appropriate
    // ("a combination of icXformLutColorimetric with icXformLutSpectral")
    // since LutColorimetric appears to disable use of DToB tags in some cases for some reason? 🤷
    constexpr auto icXformLutType = icXformLutColorimetric;

    icStatusCMM result {
        profileApplier.AddXform(profile, intent, icInterpLinear, nullptr, icXformLutType, useDToBTags)
    };

    if (result != icCmmStatOk) {
        return result;
    }

    result = profileApplier.Begin();

    if (result != icCmmStatOk) {
        return result;
    }

    icFloatNumber srcPixel[15];
    icFloatNumber dstPixel[15];

    int outputChannels;
    if (colourData.isDeviceToPcs()) {
        outputChannels = 3;
    } else {
        // non-exhaustive; update whenever encountering more exotic profiles
        switch (profile->m_Header.colorSpace) {
            case icSigCmykData: outputChannels = 4;
                break;
            case icSig7colorData: outputChannels = 7;
                break;
            case icSigGrayData: outputChannels = 1;
                break;
            default: outputChannels = 3;
                break;
        }
    }

    std::vector<IccConvert::ColourData::Rows> outputData;
    for (const auto &row: colourData.getInputCSVData()) {
        for (std::size_t i = 0; i < row.size(); ++i) {
            srcPixel[i] = static_cast<icFloatNumber>(row[i]);
        }

        if (!colourData.isDeviceToPcs()) {
            if (profile->m_Header.pcs == icSigLabData) {
                icLabToPcs(srcPixel);
            }
            else if (profile->m_Header.pcs == icSigXYZData) {
                icXyzToPcs(srcPixel);
            }
        }

        profileApplier.Apply(dstPixel, srcPixel);

        if (colourData.isDeviceToPcs()) {
            if (profile->m_Header.pcs == icSigLabData) {
                icLabFromPcs(dstPixel);
            }
            else if (profile->m_Header.pcs == icSigXYZData) {
                icXyzFromPcs(dstPixel);
            }
        }

        std::vector<double> outputValue;
        for (std::size_t i = 0; i < outputChannels; ++i) {
            outputValue.push_back(dstPixel[i]);
        }

        outputData.push_back(outputValue);
    }

    IccConvert::CommandLineUtility::writeTo(colourData.getOutputFile(), outputData, colourData);

    return icCmmStatOk;
}
