#ifndef COLOUR_DATA_H
#define COLOUR_DATA_H

#include <cstdint>
#include <filesystem>
#include <vector>
#include <string>

namespace IccConvert {
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

        void setInputCSVData(std::vector<Rows> const& inputData);
        void setInputCSVData(std::vector<Rows> &&inputData);
        void setOutputFile(std::filesystem::path const& file);
        void setRenderingIntent(std::uint32_t intent);
        void setDeviceToPcs(std::uint32_t deviceToPcsFlag);
        void setProfile(std::string const& profilePath);
        [[nodiscard]] std::vector<Rows> getInputCSVData() const;
        [[nodiscard]] std::filesystem::path getOutputFile() const;
        [[nodiscard]] RenderingIntent getRenderIntent() const;
        [[nodiscard]] bool isDeviceToPcs() const;
        [[nodiscard]] std::string getProfile() const;

    private:
        bool mIsDeviceToPcs{ false };
        std::vector<Rows> mInputCsvData{};
        std::filesystem::path mOutputFile{};
        RenderingIntent mRenderIntent{ RenderingIntent::RELATIVE };
        std::string mProfilePath{};
    };
}

#endif
