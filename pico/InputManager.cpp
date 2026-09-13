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
		UsbPacket packet = {
			.command = DeviceCommand::CMD_MOUSE_REPORT,
			.data.mouse = trueMouse
		};
		m_inputQueue.push_back(packet);
	};
}
void InputManager::ProcessKeyboardPayload(KeyboardPayload& keyboard) {
	UsbPacket packet = {
		.command = DeviceCommand::CMD_KEYBOARD_REPORT,
		.data.keyboard = keyboard
	};
	m_inputQueue.push_back(packet);
}
void InputManager::ProcessUsbPacket(UsbPacket& packet) {
	std::lock_guard<std::mutex> lock(m_queueMutex);
	
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
}
std::optional<UsbPacket> InputManager::GetNextPacket() {
	std::lock_guard<std::mutex> lock(m_queueMutex);

	if (m_inputQueue.empty())
		return std::nullopt;

	UsbPacket packet = std::move(m_inputQueue.front());
	m_inputQueue.pop_front();
	return payload;
}