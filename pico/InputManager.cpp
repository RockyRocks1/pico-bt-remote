#include "InputManager.hpp"


void InputManager::ProcessMousePayload(MousePayload& mouse) {
	int16_t accumulativeX = mouse.relativeX;
	int16_t accumulativeY = mouse.relativeY;

	while (accumulativeX != 0 || accumulativeY != 0) {
		int8_t relativeX = accumulativeX;
		if (accumulativeX > 127)
			relativeX = 127;
		else if (accumulativeX < -127)
			relativeX = -127;
		accumulativeX -= relativeX;

		int8_t relativeY = accumulativeY;
		if (accumulativeY > 127)
			relativeY = 127;
		else if (accumulativeY < -127)
			relativeY = -127;
		accumulativeY -= relativeY;

		MousePayload trueMouse = {
			.buttonMask = mouse.buttonMask,
			.relativeX = relativeX,
			.relativeY = relativeY,
			.relativeWheel = mouse.relativeWheel
		};
		UsbPacket packet{};
		packet.command = DeviceCommand::CMD_MOUSE_REPORT;
		packet.data.mouse = trueMouse;
		m_packetQueue.push_back(packet);
	};
}
void InputManager::ProcessKeyboardPayload(KeyboardPayload& keyboard) {
	UsbPacket packet{};
	packet.command = DeviceCommand::CMD_KEYBOARD_REPORT;
	packet.data.keyboard = keyboard;
	m_packetQueue.push_back(packet);
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