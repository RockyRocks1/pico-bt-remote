#include "InputManager.hpp"


void InputManager::ProcessMousePayload(MousePayload& mouse) {
	int16_t accumulativeX = mouse.dx;
	int16_t accumulativeY = mouse.dy;
	const int8_t MAX_STEP = m_configManager->GetConfig().maxDelta;
	do {
		int8_t dx = 0;
		if (accumulativeX > MAX_STEP)
			dx = MAX_STEP;
		else if (accumulativeX < -MAX_STEP)
			dx = -MAX_STEP;
		else
			dx = accumulativeX;
		accumulativeX -= dx;

		int8_t dy = 0;
		if (accumulativeY > MAX_STEP)
			dy = MAX_STEP;
		else if (accumulativeY < -MAX_STEP)
			dy = -MAX_STEP;
		else
			dy = accumulativeY;
		accumulativeY -= dy;

		MousePayload trueMouse = {
			.buttonMask = mouse.buttonMask,
			.dx = dx,
			.dy = dy,
			.dWheel = mouse.dWheel
		};
		UsbPacket packet{};
		packet.command = DeviceCommand::CMD_MOUSE_REPORT;
		packet.data.mouse = trueMouse;
		m_packetQueue.push_back(packet);
	} while (accumulativeX != 0 || accumulativeY != 0);
}
void InputManager::ProcessKeyboardPayload(KeyboardPayload& keyboard) {
	UsbPacket packet{};
	packet.command = DeviceCommand::CMD_KEYBOARD_REPORT;
	packet.data.keyboard = keyboard;
	m_packetQueue.push_back(packet);
}
void InputManager::ProcessConfigPayload(ConfigPayload& config) {
	m_configManager->UpdateConfig(config);
}
void InputManager::ProcessUsbPacket(UsbPacket& packet) {
	mutex_enter_blocking(&m_queueMutex);

	switch (packet.command) {
	case CMD_KEYBOARD_REPORT:
		ProcessKeyboardPayload(packet.data.keyboard);
		break;
	case CMD_MOUSE_REPORT:
		ProcessMousePayload(packet.data.mouse);
		break;
	case CMD_SET_CONFIG:
		ProcessConfigPayload(packet.data.config);
		break;
	default:
		break;
	}
	mutex_exit(&m_queueMutex);
}
std::optional<UsbPacket> InputManager::GetNextPacket() {
	mutex_enter_blocking(&m_queueMutex);
	if (m_packetQueue.empty()) {
		mutex_exit(&m_queueMutex);
		return std::nullopt;
	}

	UsbPacket packet = std::move(m_packetQueue.front());
	m_packetQueue.pop_front();
	mutex_exit(&m_queueMutex);
	return packet;
}