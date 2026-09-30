#pragma once
#include <openpnp-capture.h>
#include <context.h>
#include <memory>
#include <mutex>

struct FrameView {
	std::shared_ptr<const uint8_t[]> data;
	uint32_t width = 0;
	uint32_t height = 0;

	inline size_t GetBufferSize() const noexcept {
		return static_cast<size_t>(width) * height * 3;
	}
};
struct FrameBuffer {
	std::shared_ptr<uint8_t[]> data;
	uint32_t width = 0;
	uint32_t height = 0;

	FrameBuffer() = default;
	FrameBuffer(uint32_t width, uint32_t height) : width(width), height(height) {
		data = std::make_shared<uint8_t[]>(GetBufferSize());
	};
	inline size_t GetBufferSize() const noexcept {
		return static_cast<size_t>(width) * height * 3;
	}
	FrameView ToView() {
		return { 
			.data = data, 
			.width = width, 
			.height = height 
		};
	}
};
class VideoCapture {
public:
	VideoCapture(CapContext context, CapDeviceID device, CapFormatID format);
	FrameView GetLatestFrame();
private:
	CapContext m_context = nullptr;
	CapStream m_stream = -1;
	CapFormatInfo m_formatInfo{};

	FrameBuffer m_frontBuffer;
	FrameBuffer m_backBuffer;
	bool m_isNewFrame = false;
	std::mutex m_frameMutex;
	
	void PollCapture();
};