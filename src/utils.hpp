#pragma once
#include <cmath>
#include <limits>

namespace heart {
    // to test if two floating point numbers are equal
    static inline bool fequal(double x, double y) {
        return std::fabs(x - y) < std::numeric_limits<double>::epsilon();
    }

    // map x-y corrdinate to a 1D corrdinate
    // notice: x and y must be none zero
    static inline size_t map_xy(size_t x, size_t y, size_t xsize) {
        return x + y * xsize;
    }
}
