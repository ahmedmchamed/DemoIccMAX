#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <iostream>
#include "CommandLineUtility.h"

namespace IccRoundTrip {
    ColourData CommandLineUtility::readFrom(std::string const &fileName) {
        ColourData result{};
        std::vector<ColourData::Rows> csvData{};

        if (std::ifstream openFile{fileName}; openFile.is_open()) {
            std::string currentLine{};
            std::string rowItem{};

            while (std::getline(openFile, currentLine)) {
                std::istringstream lineStream{currentLine};
                ColourData::Rows row{};

                while (std::getline(lineStream, rowItem, ',')) {
                    if (rowItem != std::string{"-->"}) {
                        row.push_back(std::stof(rowItem));
                    }
                }

                csvData.push_back(row);
            }
        } else {
            std::cerr << "\nError opening file: " << fileName << std::endl;
        }

        result.setCSVTestData(std::move(csvData));
        return std::move(result);

    }

    CommandLineUtility::CommandLineArgValue CommandLineUtility::getCommandLineArgValue(std::string const &argKey) {
        CommandLineUtility::CommandLineArgValue result{};
        if (auto const foundKey{ mCommandLineArgs.find(argKey) }; foundKey != mCommandLineArgs.end()) {
            result = foundKey->second;
        }
        else {
            std::cerr << "Command line option key not found " << argKey << std::endl;
        }

        return result;
    }

    ColourData CommandLineUtility::parseCommandLineArgs(std::vector<std::string> const &commandLineArgs) {
        std::string argKey{};
        std::string argValue{};
        ColourData colourDataValue{};

        if (commandLineArgs.empty()) {
            std::cerr << "Request to parse command line arguments failed - no arguments to parse" << std::endl;
            return ColourData{};
        }

        // Closure used in the for loop below to set the command line
        // arg values according to a given command line option
        auto setCommandLineArgValues{
            [&colourDataValue, this]
            (std::string& argKey, std::string const& argValue) {
                // Convert command line options to lowercase so that
                // characters are all standardised, regardless of input format
                for (auto& letter : argKey) {
                    letter = std::tolower(static_cast<unsigned char>(letter));
                }

                if (argKey == std::string{ "csv" }) {
                    colourDataValue.setCSVTestData(readFrom(argValue).getCSVData());
                }
                else if (argKey == std::string{ "render_intent" }) {
                    colourDataValue.setRenderingIntent(std::stol(argValue));
                }
                else if (argKey == std::string{ "profile" }) {
                    colourDataValue.setProfile(argValue);
                }
                else if (argKey == std::string{ "devicetopcs" }) {
                    colourDataValue.setDeviceToPcs(std::stol(argValue));
                }
                else {
                    std::cerr << "Invalid command line option " << argKey << std::endl;
                }
            }
        };

        for (auto const& arg : commandLineArgs) {
            // Get start and end positions of command line option key
            const auto keyStart{ arg.find_first_of('-') + 1 };
            const auto keyEnd{ arg.find_last_of('=') - 1 };
            argKey = arg.substr(keyStart, keyEnd);

            // Get start position of command line option value
            const auto valueStart{ keyEnd + 2 };
            argValue = arg.substr(valueStart);

            setCommandLineArgValues(argKey, argValue);

            mCommandLineArgs.insert({ argKey, { argValue, colourDataValue } });
        }

        return colourDataValue;
    }
}
