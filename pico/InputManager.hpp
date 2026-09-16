#pragma once
#include "shared/shared.h"
#include "ConfigManager.hpp"
#include "pico/mutex.h"
#include <deque>
#include <optional>

class InputManager {
public:
	InputManager(ConfigManager* configManager): m_configManager(configManager) {
		mutex_init(&m_queueMutex);
	};
	std::optional<UsbPacket> GetNextPacket();
	inline bool IsPacketQueueEmpty() const noexcept { return m_packetQueue.empty(); }
	void ProcessUsbPacket(UsbPacket& packet);
private:
	std::deque<UsbPacket> m_packetQueue; // change to std::array later
	mutex_t m_queueMutex;
	ConfigManager* m_configManager;

	void ProcessMousePayload(MousePayload& mouse);
	void ProcessKeyboardPayload(KeyboardPayload& keyboard);
	void ProcessConfigPayload(ConfigPayload& config);
};