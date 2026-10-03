#pragma once
#include "FrameView.hpp"
#include "utils/Rect.hpp"
#include <cmath>
#include <algorithm>

enum class ThresholdType {
	BINARY,
	BINARY_INV,
	TRUNC,
	TO_ZERO,
	TO_ZERO_INV
};

class PixelModifier {
private:
	template<typename Func>
	static bool Map1to1(const FrameView& sourceView, FrameBuffer& destBuffer, Func iteratorFunction);
public:
	PixelModifier() = delete;
	static FrameView Crop(const FrameView& sourceView, const Rect& cropRegion);

	static bool Grayscale(const FrameView& sourceView, FrameBuffer& destBuffer);
	static bool ChannelFilter(const FrameView& sourceView, FrameBuffer& destBuffer, BgrChannel channel);
	static bool Invert(const FrameView& sourceView, FrameBuffer& destBuffer);
	static bool Threshold(const FrameView& sourceView, FrameBuffer& destBuffer, uint8_t thresholdVal, ThresholdType thresholdType);
};