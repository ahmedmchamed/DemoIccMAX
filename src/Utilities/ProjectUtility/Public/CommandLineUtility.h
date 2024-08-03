#ifndef COMMAND_LINE_UTILITY_H
#define COMMAND_LINE_UTILITY_H

// Utility
#include <string>
#include <filesystem>
#include <unordered_map>
#include <utility>
#include <vector>

// IccUtility
#include "ColourData.h"

namespace IccRoundTrip {
    class CommandLineUtility final {
    public:
        using CommandLineArgValue = std::pair<std::string, ColourData>;
        CommandLineUtility() = default;
        static ColourData readFrom(std::filesystem::path const& filePath);
        static void writeTo(
            std::filesystem::path const& filePath,
            std::vector<ColourData::Rows> const& outputData,
            ColourData const& colourData);
        CommandLineArgValue getCommandLineArgValue(std::string const& argKey);
        ColourData parseCommandLineArgs(std::vector<std::string> const& commandLineArgs);

    private:
        // Command line args - key is option name, value is
        // complex type of option value and value's data
        std::unordered_map<std::string, CommandLineArgValue> mCommandLineArgs;

    };
}

#endif
