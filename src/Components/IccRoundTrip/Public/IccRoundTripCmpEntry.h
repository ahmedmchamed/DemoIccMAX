#ifndef ICC_ROUND_TRIP_CMP_ENTRY_H
#define ICC_ROUND_TRIP_CMP_ENTRY_H

#include <cstdint>
#include <vector>
#include <string>

namespace IccRoundTrip {
    int run(std::uint16_t const argCount, std::vector<std::string> const& args);
}

#endif
