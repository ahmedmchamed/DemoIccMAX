#ifndef COMMAND_LINE_UTILITY_H
#define COMMAND_LINE_UTILITY_H

// Utility
#include <string>
#include <unordered_map>
#include <vector>

// IccUtility
#include "ColourData.h"

namespace IccRoundTrip {
    class CommandLineUtility final {
        private:
        std::unordered_map<std::string, std::string> mCommandLineArgs;

        public:
        CommandLineUtility() = default;
        ColourData readFrom(std::string const& fileName);
        void writeTo(std::string const& fileName);
        void parseCommandLineArgs(std::vector<std::string> const& commandLineArgs);
    };
}

#endif
