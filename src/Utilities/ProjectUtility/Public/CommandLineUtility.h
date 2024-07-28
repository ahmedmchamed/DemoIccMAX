#ifndef COMMAND_LINE_UTILITY_H
#define COMMAND_LINE_UTILITY_H

// Utility
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

// IccUtility
#include "ColourData.h"

namespace IccRoundTrip {
    class CommandLineUtility final {
    public:
        using CommandLineArgValue = std::pair<std::string, ColourData>;
        CommandLineUtility() = default;
        static ColourData readFrom(std::string const& fileName);
        CommandLineArgValue getCommandLineArgValue(std::string const& argKey);
        ColourData parseCommandLineArgs(std::vector<std::string> const& commandLineArgs);

    private:
        // Command line args - key is option name, value is
        // complex type of option value and value's data
        std::unordered_map<std::string, CommandLineArgValue> mCommandLineArgs;

    };
}

#endif
