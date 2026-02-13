#include "Config.h"

#include <iostream>

namespace Utils {
    void Config::setInputSingleData(const std::string &input) {
        std::istringstream stream(input);
        std::string word;
        std::vector<double> numbers(15, 0);

        int i = 0;
        while (stream >> word) {
            numbers[i] = std::stod(word);
            i++;
        }

        mInputSingleData = numbers;
    }

    void Config::setInputCSVData(std::vector<Row> const& inputData) {
        mInputCsvData = inputData;
    }

    void Config::setInputCSVData(std::vector<Row> &&inputData) {
        mInputCsvData = std::move(inputData);
    }

    void Config::setOutputFile(std::filesystem::path const &file) {
        mOutputFile = file;
    }

    void Config::setRenderingIntent(std::uint32_t intent) {
        mRenderIntent = static_cast<RenderingIntent>(intent);
    }

    void Config::setDeviceToPcs(std::uint32_t const deviceToPcsFlag) {
        mIsDeviceToPcs = static_cast<bool>(deviceToPcsFlag);
    }

    void Config::setProfile(std::string const &profilePath) {
        mProfilePath = profilePath;
    }

    std::vector<double> Config::getInputSingleData() const {
        return mInputSingleData;
    }

    std::vector<Config::Row> Config::getInputCSVData() const {
        return mInputCsvData;
    }

    std::filesystem::path Config::getOutputFile() const {
        return mOutputFile;
    }

    Config::RenderingIntent Config::getRenderIntent() const {
        return mRenderIntent;
    }

    bool Config::isDeviceToPcs() const {
        return mIsDeviceToPcs;
    }

    std::string Config::getProfile() const {
        return mProfilePath;
    }
}
