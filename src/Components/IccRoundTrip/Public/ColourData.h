#ifndef COLOUR_DATA_H
#define COLOUR_DATA_H

#include <vector>

struct ColourData {
    using Rows = std::vector<float>;
    bool isDeviceToPcs{ false };
    std::vector<Rows> csvData{};
};

#endif
