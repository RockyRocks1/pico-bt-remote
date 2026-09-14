#include "UsbStreamListener.hpp"

void UsbStreamListener::PollStreamAndPipeTo(InputManager& inputManager) {
    while (true) {
        uint8_t* packetBuffer = reinterpret_cast<uint8_t*>(&m_incomingPacket);

        int byteIn = getchar_timeout_us(0);
        if (byteIn == -2)
            break;

        packetBuffer[m_bytesRead++] = static_cast<uint8_t>(byteIn);

        if (m_bytesRead == sizeof(UsbPacket)) {
            m_bytesRead = 0;
            inputManager.ProcessUsbPacket(m_incomingPacket);
        }
    }
}