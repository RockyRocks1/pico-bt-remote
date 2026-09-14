#include "UsbStreamWriter.hpp"
#include <iostream>
#include <thread>

int main() {
    std::string portName = "COM3";
    uint32_t baudRate = 115200;
    UsbStreamWriter writer;

    if (!writer.Open(portName, baudRate)) {
        std::cerr << "Failed to open serial port" << std::endl;
        return -1;
    }

    POINT previousPoint;
    GetCursorPos(&previousPoint);
    bool previousMouseState = !!(GetAsyncKeyState(VK_LBUTTON) & 0x8000);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        POINT currentPoint;
        bool currentMouseState = !!(GetAsyncKeyState(VK_LBUTTON) & 0x8000);
        if (!GetCursorPos(&currentPoint))
            continue; 

        int16_t deltaX = static_cast<int16_t>(currentPoint.x - previousPoint.x);
        int16_t deltaY = static_cast<int16_t>(currentPoint.y - previousPoint.y);

        if (deltaX != 0 || deltaY != 0 || currentMouseState != previousMouseState) {
            UsbPacket packet{};
            packet.command = CMD_MOUSE_REPORT;
            packet.data.mouse.buttonMask = currentMouseState;
            packet.data.mouse.relativeX = deltaX;
            packet.data.mouse.relativeY = deltaY;

            writer.Write(packet);
        }
        previousPoint = currentPoint;
        previousMouseState = currentMouseState;
    }
}