#include "ColourData.h"

namespace IccRoundTrip {
    void ColourData::setInputCSVData(std::vector<Rows> const& inputData) {
        mInputCsvData = inputData;
    }

    void ColourData::setInputCSVData(std::vector<Rows> &&inputData) {
        mInputCsvData = std::move(inputData);
    }

    void ColourData::setOutputFile(std::filesystem::path const &file) {
        mOutputFile = file;
    }

    void ColourData::setRenderingIntent(std::uint32_t intent) {
        mRenderIntent = static_cast<RenderingIntent>(intent);
    }

    void ColourData::setDeviceToPcs(std::uint32_t deviceToPcsFlag) {
        mIsDeviceToPcs = static_cast<bool>(deviceToPcsFlag);
    }

    void ColourData::setProfile(std::string const &profilePath) {
        mProfilePath = profilePath;
    }

    std::vector<ColourData::Rows> ColourData::getInputCSVData() const {
        return mInputCsvData;
    }

    std::string ColourData::getOutputFile() const {
        return mOutputFile;
    }

    ColourData::RenderingIntent ColourData::getRenderIntent() const {
        return mRenderIntent;
    }

    bool ColourData::isDeviceToPcs() const {
        return mIsDeviceToPcs;
    }

    std::string ColourData::getProfile() const {
        return mProfilePath;
    }
}
