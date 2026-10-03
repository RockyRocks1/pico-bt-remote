#pragma once
#include "Color.hpp"
#include <vector>
#include <memory>

enum class PixelFormat : uint8_t {
    Bgr8,
    Gray8
};

struct FrameView {
    std::shared_ptr<const uint8_t[]> data;
    uint32_t width = 0;
    uint32_t height = 0;
    PixelFormat format = PixelFormat::Bgr8;

    inline uint8_t GetBytesPerPixel() const noexcept {
        switch (format) {
        case PixelFormat::Bgr8:
            return 3;
        case PixelFormat::Gray8:
            return 1;
        default:
            return 0;
        };
    }
    inline size_t GetPixelCount() const noexcept {
        return static_cast<size_t>(width * height);
    }
    inline size_t GetBufferSize() const noexcept {
        return GetPixelCount() * GetBytesPerPixel();
    }
};

struct FrameBuffer {
    std::shared_ptr<uint8_t[]> data;
    uint32_t width = 0;
    uint32_t height = 0;
    PixelFormat format = PixelFormat::Bgr8;

    FrameBuffer() = default;
    FrameBuffer(uint32_t width, uint32_t height) : width(width), height(height) {
        data = std::make_shared<uint8_t[]>(GetBufferSize());
    };

    FrameView ToView() const noexcept {
        return {
            .data = data,
            .width = width,
            .height = height,
            .format = format
        };
    }
    
    inline size_t GetBytesPerPixel() const noexcept {
        switch (format) {
        case PixelFormat::Bgr8:
            return 3;
        case PixelFormat::Gray8:
            return 1;
        default:
            return 0;
        };
    }
    inline size_t GetPixelCount() const noexcept {
        return static_cast<size_t>(width * height);
    }
    inline size_t GetBufferSize() const noexcept {
        return GetPixelCount() * GetBytesPerPixel();
    }
};
