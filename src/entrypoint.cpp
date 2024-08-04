#include <cstdint>
#include <vector>

#include "ColourData.h"
#include "CommandLineUtility.h"
#include "IccCmm.h"
#include "IccProfile.h"
#include "icProfileHeader.h"

int main(int argc, char* argv[]) {
    std::vector<std::string> commandLineArgs{};
    // Make sure to skip program name itself by
    // indexing from 1
    for (std::uint32_t i{ 1 }; i < argc; ++i) {
        commandLineArgs.emplace_back(argv[i]);
    }

    IccConvert::CommandLineUtility utility{};
    IccConvert::ColourData const colourData{ utility.parseCommandLineArgs(commandLineArgs)};

    CIccProfile *profile = ReadIccProfile(colourData.getProfile().c_str());
    auto intent = static_cast<icRenderingIntent>(static_cast<std::uint32_t>(colourData.getRenderIntent()));
    auto useMPE = false; // whatever this is...

    if (!profile) {
        return icCmmStatCantOpenProfile;
    }

    if (profile->m_Header.deviceClass != icSigInputClass &&
        profile->m_Header.deviceClass != icSigDisplayClass &&
        profile->m_Header.deviceClass != icSigOutputClass &&
        profile->m_Header.deviceClass != icSigColorSpaceClass) {
        return icCmmStatInvalidProfile;
        }

    CIccCmm profileApplier(
        colourData.isDeviceToPcs() ? icSigUnknownData : icSigLabData,
        colourData.isDeviceToPcs() ? icSigLabData : profile->m_Header.colorSpace,
        colourData.isDeviceToPcs()
    );

    icStatusCMM result{ profileApplier.AddXform(profile, intent, icInterpLinear, nullptr, icXformLutColorimetric, useMPE) };

    if (result!=icCmmStatOk) {
        return result;
    }

    result = profileApplier.Begin();

    if (result != icCmmStatOk) {
        return result;
    }

    icFloatNumber sPixel[15];
    icFloatNumber devPcs[15];

    std::vector<IccConvert::ColourData::Rows> outputData;
    for (const auto &row: colourData.getInputCSVData()) {
        for (std::size_t i = 0; i < row.size(); ++i) {
            sPixel[i] = row[i];
        }

        if (!colourData.isDeviceToPcs()) {
            icLabToPcs(sPixel);
        }

        profileApplier.Apply(devPcs, sPixel); //Convert device value to pcs from input table

        if (colourData.isDeviceToPcs()) {
            icLabFromPcs(devPcs);
        }

        std::vector<float> outputValue;
        for (const auto channel: devPcs) {
            outputValue.push_back(channel);
        }

        outputData.push_back(outputValue);
    }

    IccConvert::CommandLineUtility::writeTo(colourData.getOutputFile(), outputData, colourData);

    return icCmmStatOk;
}
