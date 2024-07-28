#include "ColourData.h"

namespace IccRoundTrip {
    void ColourData::setCSVTestData(std::vector<Rows> const& testData) {
        mCsvData = testData;
    }

    void ColourData::setCSVTestData(std::vector<Rows> &&testData) {
        mCsvData = std::move(testData);
    }

    void ColourData::setRenderingProfile(std::uint32_t profile) {
        mRenderIntent = static_cast<RenderingIntent>(profile);
    }

    void ColourData::setDeviceToPcs(std::uint32_t deviceToPcsFlag) {
        mIsDeviceToPcs = static_cast<bool>(deviceToPcsFlag);
    }

    std::vector<ColourData::Rows> ColourData::getCSVData() const {
        return mCsvData;
    }

    ColourData::RenderingIntent ColourData::getProfile() const {
        return mRenderIntent;
    }
}
