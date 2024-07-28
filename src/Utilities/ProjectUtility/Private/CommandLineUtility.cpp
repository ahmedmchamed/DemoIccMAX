#include <fstream>
#include <sstream>
#include <iostream>
#include "CommandLineUtility.h"

namespace IccRoundTrip {
    ColourData CommandLineUtility::readFrom(std::string const &fileName) {
        ColourData colourData{};

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

                colourData.csvData.push_back(row);
            }
        } else {
            std::cerr << "\nError opening file: " << fileName << std::endl;
        }

        return std::move(colourData);

    }

    std::string CommandLineUtility::getFileName() {
        std::string fileName{};
        const auto filenameArgPos{ mCommandLineArgs.find(std::string{ "inputfile" }) };
        if (filenameArgPos != mCommandLineArgs.end()) {
            fileName = filenameArgPos->second;
        }
        else {
            std::cerr << "No file input was found for filename " << fileName << std::endl;
        }

        return fileName;
    }

    void CommandLineUtility::writeTo(std::string const &fileName) {
    }

    void CommandLineUtility::parseCommandLineArgs(std::vector<std::string> const &commandLineArgs) {
        std::string argKey{};
        std::string argValue{};

        if (commandLineArgs.empty()) {
            std::cerr << "Request to parse command line arguments failed - no arguments to parse" << std::endl;
            return;
        }

        for (auto const& arg : commandLineArgs) {
            const auto keyStart{ arg.find_first_of('-') + 1 };
            const auto keyEnd{ arg.find_last_of('=') - 1 };
            argKey = arg.substr(keyStart, keyEnd);

            const auto valueStart{ keyEnd + 2 };
            argValue = arg.substr(valueStart);

            mCommandLineArgs.insert({ argKey, argValue });
        }
    }
}
