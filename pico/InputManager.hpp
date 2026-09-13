#include "shared/shared.h"
#include <deque>
#include <mutex>
#include <optional>

class InputManager {
public:
	InputManager() {
		
	};
	std::optional<UsbPacket> GetNextPacket();
	void ProcessUsbPacket(UsbPacket& packet);
private:
	std::deque<UsbPacket> m_packetQueue;
	std::mutex m_queueMutex;
	void ProcessMousePayload(MousePayload& mouse);
	void ProcessKeyboardPayload(KeyboardPayload& keyboard);
}