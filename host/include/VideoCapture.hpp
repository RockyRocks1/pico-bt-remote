#pragma once
#include <openpnp-capture.h>
#include <context.h>
#include <memory>
#include <mutex>
#include "utils/pixel/FrameView.hpp"

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