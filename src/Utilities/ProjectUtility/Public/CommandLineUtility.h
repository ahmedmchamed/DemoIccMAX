#ifndef COMMAND_LINE_UTILITY_H
#define COMMAND_LINE_UTILITY_H

// Utility
#include <string>
#include <unordered_map>

// IccUtility
#include "ColourData.h"

namespace IccRoundTrip {
    class CommandLineUtility final {
        private:
        std::unordered_map<std::string, ColourData> mCommandLineArgValues;

        public:
        CommandLineUtility() = default;
        ColourData readFrom(std::string const& fileName);
        void writeTo(std::string const& fileName);
    };
}

#endif
