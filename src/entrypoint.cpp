#include "IccRoundTripCmpEntry.h"
#include <cstdint>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<std::string> commandLineArgs{};
    // Make sure to skip program name itself by
    // indexing from 1
    for (std::uint32_t i{ 1 }; i < argc; ++i) {
        commandLineArgs.emplace_back(argv[i]);
    }

    IccRoundTrip::run(argc, commandLineArgs);
}