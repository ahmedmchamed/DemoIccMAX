#include "ColourData.h"

namespace IccRoundTrip {
    void ColourData::setCSVTestData(std::vector<Rows> const& testData) {
        mCsvData = testData;
    }

    void ColourData::setCSVTestData(std::vector<Rows> &&testData) {
        mCsvData = std::move(testData);
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

    std::vector<ColourData::Rows> ColourData::getCSVData() const {
        return mCsvData;
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
