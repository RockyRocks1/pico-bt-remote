#include <utils/pixel/PixelModifier.hpp>

template<typename Func>
bool PixelModifier::Map1to1(const FrameView& sourceView, FrameBuffer& destBuffer, Func iteratorFunction) {
	if (sourceView.width != destBuffer.width || sourceView.height != destBuffer.height)
		return false;
	if (!sourceView.data || !destBuffer.data.data())
		return false;

	const uint32_t sourceBytesPerPixel = sourceView.GetBytesPerPixel();
	const uint32_t destBytesPerPixel = destBuffer.GetBytesPerPixel();

	for (int y = 0; y < sourceView.height; y++) {
		const uint8_t* pSourceRow = static_cast<const uint8_t*>(sourceView.data.get() + sourceView.width * y);
		uint8_t* pDestRow = destBuffer.data.data() + destBuffer.width * y;

		const uint8_t* pSourcePixel = pSourceRow;
		uint8_t* pDestPixel = pDestRow;
		for (int x = 0; x < sourceView.width; x++) {
			iteratorFunction(pSourcePixel, pDestPixel);
			pSourcePixel += sourceBytesPerPixel;
			pDestPixel += destBytesPerPixel;
		}
	}
	return true;
}

FrameView PixelModifier::Crop(const FrameView& sourceView, const Rect& cropRegion) {
	if (cropRegion.width <= 0 || cropRegion.height <= 0 || cropRegion.x + cropRegion.width > sourceView.width || cropRegion.y + cropRegion.height > sourceView.height)
		return {};
	size_t byteOffset = (static_cast<size_t>(cropRegion.y) * sourceView.width) + (static_cast<size_t>(cropRegion.x) * sourceView.GetBytesPerPixel());
	std::shared_ptr<const uint8_t[]> croppedData(sourceView.data, sourceView.data.get() + byteOffset);

	return FrameView{
		.data = croppedData,
		.width = cropRegion.width,
		.height = cropRegion.height,
		.format = sourceView.format
	};
}

bool PixelModifier::Grayscale(const FrameView& sourceView, FrameBuffer& destBuffer) {
	if (sourceView.format != PixelFormat::Bgr8)
		return false;

	destBuffer.width = sourceView.width;
	destBuffer.height = sourceView.height;
	destBuffer.format = PixelFormat::Gray8;
	destBuffer.data = std::make_shared<uint8_t[]>(destBuffer.GetBufferSize());

	auto grayscale = [](const uint8_t* src, uint8_t* dest) {
		*dest = (src[static_cast<uint8_t>(BgrChannel::R)] * 54 +
			src[static_cast<uint8_t>(BgrChannel::G)] * 183 +
			src[static_cast<uint8_t>(BgrChannel::B)] * 18) >> 8;
		};

	return Map1to1(sourceView, destBuffer, grayscale);
}
bool PixelModifier::ChannelFilter(const FrameView& sourceView, FrameBuffer& destBuffer, BgrChannel channel) {
	if (sourceView.format != PixelFormat::Bgr8)
		return false;
	
	destBuffer.width = sourceView.width;
	destBuffer.height = sourceView.height;
	destBuffer.format = PixelFormat::Gray8;
	destBuffer.data = std::make_shared<uint8_t[]>(destBuffer.GetBufferSize());

	switch (channel) {
	case BgrChannel::B: {
		auto filterB = [](const uint8_t* src, uint8_t* dest) {
			*dest = src[static_cast<uint8_t>(BgrChannel::B)];
			};
		return Map1to1(sourceView, destBuffer, filterB);
	}
	case BgrChannel::G: {
		auto filterG = [](const uint8_t* src, uint8_t* dest) {
			*dest = src[static_cast<uint8_t>(BgrChannel::G)];
			};
		return Map1to1(sourceView, destBuffer, filterG);
	}
	case BgrChannel::R: {
		auto filterR = [](const uint8_t* src, uint8_t* dest) {
			*dest = src[static_cast<uint8_t>(BgrChannel::R)];
			};
		return Map1to1(sourceView, destBuffer, filterR);
	}
	default:
		return false;
	}
	

}

bool PixelModifier::Invert(const FrameView& sourceView, FrameBuffer& destBuffer) {
	destBuffer.width = sourceView.width;
	destBuffer.height = sourceView.height;
	destBuffer.format = sourceView.format;
	destBuffer.data = std::make_shared<uint8_t[]>(destBuffer.GetBufferSize());

	switch (sourceView.format) {
	case PixelFormat::Bgr8: {
		auto invertBgra = [](const uint8_t* src, uint8_t* dest) {
			uint32_t srcPixel = *reinterpret_cast<const uint32_t*>(src);
			*reinterpret_cast<uint32_t*>(dest) = srcPixel ^ 0xFFFFFF;
			};
		return Map1to1(sourceView, destBuffer, invertBgra);
	}
	case PixelFormat::Gray8: {
		auto invertGray = [](const uint8_t* src, uint8_t* dest) {
			*dest = ~src[0];
			};
		return Map1to1(sourceView, destBuffer, invertGray);
	}
	default:
		return false;
	}
}
bool PixelModifier::Threshold(const FrameView& sourceView, FrameBuffer& destBuffer, uint8_t thresholdVal, ThresholdType thresholdType) {
	if (sourceView.format != PixelFormat::Gray8)
		return false;

	destBuffer.width = sourceView.width;
	destBuffer.height = sourceView.height;
	destBuffer.format = sourceView.format;
	destBuffer.data = std::make_shared<uint8_t[]>(destBuffer.GetBufferSize());

	switch (thresholdType) {
	case ThresholdType::BINARY: {
		auto binary = [thresholdVal](const uint8_t* src, uint8_t* dest) {
			*dest = (src[0] < thresholdVal) ? 0 : 255;
			};
		return Map1to1(sourceView, destBuffer, binary);
	}
	case ThresholdType::BINARY_INV: {
		auto binaryInv = [thresholdVal](const uint8_t* src, uint8_t* dest) {
			*dest = (src[0] < thresholdVal) ? 255 : 0;
			};
		return Map1to1(sourceView, destBuffer, binaryInv);
	}
	case ThresholdType::TRUNC: {
		auto trunca = [thresholdVal](const uint8_t* src, uint8_t* dest) {
			*dest = (src[0] > thresholdVal) ? thresholdVal : src[0];
			};
		return Map1to1(sourceView, destBuffer, trunca);
	}
	case ThresholdType::TO_ZERO: {
		auto toZero = [thresholdVal](const uint8_t* src, uint8_t* dest) {
			*dest = (src[0] < thresholdVal) ? 0 : src[0];
			};
		return Map1to1(sourceView, destBuffer, toZero);
	}
	case ThresholdType::TO_ZERO_INV: {
		auto toZeroInv = [thresholdVal](const uint8_t* src, uint8_t* dest) {
			*dest = (src[0] > thresholdVal) ? 0 : src[0];
			};
		return Map1to1(sourceView, destBuffer, toZeroInv);
	}
	default:
		return false;
	};
}