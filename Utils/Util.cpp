#include "Util.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Utils {
    Util::CommandLineArgValue Util::getCommandLineArgValue(std::string const &argKey) {
        Util::CommandLineArgValue result{};
        if (auto const foundKey{mCommandLineArgs.find(argKey)}; foundKey != mCommandLineArgs.end()) {
            result = foundKey->second;
        } else {
            std::cerr << "Command line option key not found " << argKey << std::endl;
        }

        return result;
    }

    Config Util::parseCommandLine(std::vector<std::string> const &commandLineArgs) {
        std::string argKey{};
        std::string argValue{};
        Config config{};

        if (commandLineArgs.empty()) {
            std::cerr << "Request to parse command line arguments failed - no arguments to parse" << std::endl;
            return Config{};
        }

        // Closure used in the for loop below to set the command line
        // arg values according to a given command line option
        auto setCommandLineArgValues{
            [&config, this]
            (std::string &argKey, std::string const &argValue) {
                // Convert command line options to lowercase so that
                // characters are all standardised, regardless of input format
                for (auto &letter: argKey) {
                    letter = std::tolower(static_cast<unsigned char>(letter));
                }

                if (argKey == std::string{"single_data"}) {
                    config.setInputSingleData(argValue);
                } else if (argKey == std::string{"input_file"}) {
                    config.setInputCSVData(Util::readFrom(argValue).getInputCSVData());
                } else if (argKey == std::string{"render_intent"}) {
                    config.setRenderingIntent(std::stol(argValue));
                } else if (argKey == std::string{"profile"}) {
                    config.setProfile(argValue);
                } else if (argKey == std::string{"devicetopcs"}) {
                    config.setDeviceToPcs(std::stol(argValue));
                } else if (argKey == std::string{"output_to"}) {
                    config.setOutputFile(argValue);
                } else {
                    std::cerr << "Invalid command line option " << argKey << std::endl;
                }
            }
        };

        for (auto const &arg: commandLineArgs) {
            // Get start and end positions of command line option key
            const auto keyStart{arg.find_first_of('-') + 1};
            const auto keyEnd{arg.find_last_of('=') - 1};
            argKey = arg.substr(keyStart, keyEnd);

            // Get start position of command line option value
            const auto valueStart{keyEnd + 2};
            argValue = arg.substr(valueStart);

            setCommandLineArgValues(argKey, argValue);

            mCommandLineArgs.insert({argKey, {argValue, config}});
        }

        return config;
    }

    Config Util::readFrom(std::filesystem::path const &filePath) {
        Config result{};
        std::vector<Config::Row> csvData{};

        if (std::ifstream openFile{filePath}; openFile.is_open()) {
            std::string currentLine{};
            std::string rowItem{};

            while (std::getline(openFile, currentLine)) {
                std::istringstream lineStream{currentLine};
                Config::Row row{};

                while (std::getline(lineStream, rowItem, ',')) {
                    row.push_back(std::stod(rowItem));
                }

                csvData.push_back(row);
            }
        } else {
            std::cerr << "\nError opening file: " << filePath.filename() << std::endl;
        }

        result.setInputCSVData(std::move(csvData));
        return std::move(result);
    }

    void Util::writeTo(
        std::filesystem::path const &filePath,
        std::vector<Config::Row> const &outputData,
        Config const &colourData) {
        std::ofstream fileStream{filePath};
        auto const inputData{colourData.getInputCSVData()};

        for (std::uint32_t i{0}, j{0}; i < outputData.size() && j < inputData.size(); ++i, ++j) {
            for (std::uint32_t l{0}; l < inputData.at(j).size(); ++l) {
                fileStream << std::defaultfloat << inputData.at(j).at(l) << ',';
            }
            fileStream << "-->";

            for (std::uint32_t k{0}; k < outputData.at(i).size(); ++k) {
                fileStream << ',' << std::fixed << outputData.at(i).at(k);
            }
            fileStream << std::endl;
        }

        fileStream.close();
    }
}
