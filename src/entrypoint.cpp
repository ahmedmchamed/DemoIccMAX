#include "IccRoundTripCmpEntry.h"
#include <cstdint>
#include <vector>

std::uint32_t main(std::uint32_t const argc, char* argv[]) {
    std::vector<std::string> commandLineArgs{};
    for (std::uint32_t i{ 0 }; i < argc; ++i) {
        commandLineArgs.emplace_back(argv[i]);
    }

    IccRoundTrip::run(argc, commandLineArgs);
}