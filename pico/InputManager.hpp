#pragma once
#include "shared/shared.h"
#include "pico/mutex.h"
#include <deque>
#include <optional>

class InputManager {
public:
	InputManager() {
		mutex_init(&m_queueMutex);
	};
	std::optional<UsbPacket> GetNextPacket();
	inline bool IsPacketQueueEmpty() const noexcept { return m_packetQueue.empty(); }
	void ProcessUsbPacket(UsbPacket& packet);
private:
	std::deque<UsbPacket> m_packetQueue;
	mutex_t m_queueMutex;
	void ProcessMousePayload(MousePayload& mouse);
	void ProcessKeyboardPayload(KeyboardPayload& keyboard);
};