#include "UsbStreamWriter.hpp"

bool UsbStreamWriter::Open(const std::string& portName, uint32_t baudRate) {
    std::string winPort = "\\\\.\\" + portName;
    m_portName = portName;
    m_baudRate = baudRate;
    m_portHandle = CreateFileA(winPort.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_portHandle == INVALID_HANDLE_VALUE)
        return false;

    DCB dcbSerialParams{};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(m_portHandle, &dcbSerialParams))
        return false;

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_ENABLE;

    if (!SetCommState(m_portHandle, &dcbSerialParams))
        return false;
    return true;
}
bool UsbStreamWriter::Write(const UsbPacket& packet) {
    if (m_portHandle == INVALID_HANDLE_VALUE)
        return false;

    DWORD bytesWritten;
    return WriteFile(m_portHandle, &packet, sizeof(packet), &bytesWritten, NULL);
}
