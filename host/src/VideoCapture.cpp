#include "VideoCapture.hpp"

VideoCapture::VideoCapture(CapContext context, CapDeviceID device, CapFormatID format): m_context(context) {
	m_stream = Cap_openStream(m_context, device, format);
	Cap_getFormatInfo(m_context, device, format, &m_formatInfo);
	
	m_frontBuffer = FrameBuffer(m_formatInfo.width, m_formatInfo.height);
	m_backBuffer = FrameBuffer(m_formatInfo.width, m_formatInfo.height);
}

void VideoCapture::PollCapture() {
	if (!Cap_hasNewFrame(m_context, m_stream))
		return;

	std::lock_guard lock(m_frameMutex);
	uint8_t* bufferPointer = &m_backBuffer.data.get()[0];
	Cap_captureFrame(m_context, m_stream, bufferPointer, m_backBuffer.GetBufferSize());
	m_isNewFrame = true;
}
FrameView VideoCapture::GetLatestFrame() {
	std::lock_guard lock(m_frameMutex);
	if (m_isNewFrame) {
		std::swap(m_frontBuffer, m_backBuffer);
		m_isNewFrame = false;
	}
	return m_frontBuffer.ToView();
}