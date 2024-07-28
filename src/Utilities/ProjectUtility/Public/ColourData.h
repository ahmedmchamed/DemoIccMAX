#ifndef COLOUR_DATA_H
#define COLOUR_DATA_H

#include <cstdint>
#include <vector>
#include <string>

namespace IccRoundTrip {
    class ColourData {
    public:
        ColourData() = default;

        using Rows = std::vector<float>;

        enum class RenderingIntent : std::uint16_t {
            PERCEPTUAL=0,
            RELATIVE=1,
            SATURATION=2,
            ABSOLUTE=3
        };

        void setCSVTestData(std::vector<Rows> const& testData);
        void setCSVTestData(std::vector<Rows> &&testData);
        void setRenderingIntent(std::uint32_t intent);
        void setDeviceToPcs(std::uint32_t deviceToPcsFlag);
        void setProfile(std::string const& profilePath);
        [[nodiscard]] std::vector<Rows> getCSVData() const;
        [[nodiscard]] RenderingIntent getRenderIntent() const;
        [[nodiscard]] bool isDeviceToPcs() const;
        [[nodiscard]] std::string getProfile() const;

    private:
        bool mIsDeviceToPcs{ false };
        std::vector<Rows> mCsvData{};
        RenderingIntent mRenderIntent{ RenderingIntent::RELATIVE };
        std::string mProfilePath{};
    };
}

#endif
