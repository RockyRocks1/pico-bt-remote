#include "InputManager.hpp"


void InputManager::ProcessMousePayload(MousePayload& mouse) {
	int16_t accumulativeX = mouse.relativeX;
	int16_t accumulativeY = mouse.relativeY;
	const int8_t MAX_STEP = 127;
	do {
		int8_t relativeX = accumulativeX;
		if (accumulativeX > MAX_STEP)
			relativeX = MAX_STEP;
		else if (accumulativeX < -MAX_STEP)
			relativeX = -MAX_STEP;
		accumulativeX -= relativeX;

		int8_t relativeY = accumulativeY;
		if (accumulativeY > MAX_STEP)
			relativeY = MAX_STEP;
		else if (accumulativeY < -MAX_STEP)
			relativeY = -MAX_STEP;
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
	} while (accumulativeX != 0 || accumulativeY != 0);
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