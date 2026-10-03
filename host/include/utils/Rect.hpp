#pragma once
#include <stdint.h>

struct Rect {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};
struct Size2D {
    uint32_t width = 0;
    uint32_t height = 0;

    inline bool operator==(const Size2D& other) const {
        return width == other.width && height == other.height;
    }
};