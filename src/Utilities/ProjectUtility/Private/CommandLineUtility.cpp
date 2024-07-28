#include "CommandLineUtility.h"

namespace IccRoundTrip {
    ColourData CommandLineUtility::readFrom(std::string const &fileName) {

    }

    void CommandLineUtility::writeTo(std::string const &fileName) {
    }

    void CommandLineUtility::parseCommandLineArgs(std::vector<std::string> const &commandLineArgs) {
        std::string argKey{};
        std::string argValue{};

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
