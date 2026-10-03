#pragma once
#include <stdint.h>

enum class BgrChannel : uint8_t {
    B = 0,
    G = 1,
    R = 2,
};
enum class RgbChannel : uint8_t {
    R = 0,
    G = 1,
    B = 2,
};

struct ColorRgba;
struct ColorBgra;

struct ColorRgba {
    uint8_t r = 0, g = 0, b = 0, a = 255;
    bool operator==(const ColorRgba&) const = default;
    bool operator==(const ColorBgra& other) const;
    template <typename T>
    inline bool IsCloseTo(const T& other, int variation = 0) const;
};
struct ColorBgra {
    uint8_t b = 0, g = 0, r = 0, a = 255;
    bool operator==(const ColorBgra&) const = default;
    bool operator==(const ColorRgba& other) const { return other == *this; }
};
template <typename T>
inline bool ColorRgba::IsCloseTo(const T& other, int variation) const {
    if (variation == 0)
        return *this == other;

    return std::abs(static_cast<int>(r) - other.r) <= variation &&
        std::abs(static_cast<int>(g) - other.g) <= variation &&
        std::abs(static_cast<int>(b) - other.b) <= variation;
}
inline bool ColorRgba::operator==(const ColorBgra& other) const {
    return r == other.r && g == other.g && b == other.b;
};
