#ifndef COLOUR_DATA_H
#define COLOUR_DATA_H

#include <cstdint>
#include <filesystem>
#include <vector>
#include <string>

namespace Utils {
    class Config {
    public:
        Config() = default;

        using Row = std::vector<double>;

        enum class RenderingIntent : std::uint16_t {
            PERCEPTUAL=0,
            RELATIVE=1,
            SATURATION=2,
            ABSOLUTE=3
        };

        void setInputSingleData(const std::string & string);
        void setInputCSVData(std::vector<Row> const& inputData);
        void setInputCSVData(std::vector<Row> &&inputData);
        void setOutputFile(std::filesystem::path const& file);
        void setRenderingIntent(std::uint32_t intent);
        void setDeviceToPcs(std::uint32_t deviceToPcsFlag);
        void setProfile(std::string const& profilePath);
        [[nodiscard]] std::vector<double> getInputSingleData() const;
        [[nodiscard]] std::vector<Row> getInputCSVData() const;
        [[nodiscard]] std::filesystem::path getOutputFile() const;
        [[nodiscard]] RenderingIntent getRenderIntent() const;
        [[nodiscard]] bool isDeviceToPcs() const;
        [[nodiscard]] std::string getProfile() const;

    private:
        bool mIsDeviceToPcs{ false };
        std::vector<double> mInputSingleData{};
        std::vector<Row> mInputCsvData{};
        std::filesystem::path mOutputFile{};
        RenderingIntent mRenderIntent{ RenderingIntent::RELATIVE };
        std::string mProfilePath{};
    };
}

#endif
