#include "UsbStreamWriter.hpp"
#include <iostream>
#include <thread>

const double DELTA_TO_PIXEL = 2.786;

void StartCalibrationTest(UsbStreamWriter& writer) {
    const int startStep = 1;
    const int endStep = 127;
    const int sleepTime = 100;
    for (int currentStep = startStep; currentStep <= endStep; currentStep++) {
        UsbPacket mousePacket{};
        mousePacket.command = CMD_MOUSE_REPORT;
        mousePacket.data.mouse.buttonMask = 0;
        mousePacket.data.mouse.dy = 0;

        mousePacket.data.mouse.dx = -endStep;
        writer.Write(mousePacket);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    
        mousePacket.data.mouse.dx = currentStep;
        writer.Write(mousePacket);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    };
}


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
    char startTestButton = 'T';
    bool isStartTestButtonHeld = false;
    ConfigPayload hi{};
    hi.maxDelta = 100;
    UsbPacket configPacket{};
    configPacket.command = CMD_SET_CONFIG;
    configPacket.data.config = hi;
    writer.Write(configPacket);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (GetAsyncKeyState(startTestButton) & 0x8000) {
            if (!isStartTestButtonHeld) {
                std::cout << "Starting Test" << std::endl;
                StartCalibrationTest(writer);
            }
            isStartTestButtonHeld = true;
            continue;
        }
        else
            isStartTestButtonHeld = false;

        POINT currentPoint;
        if (!GetCursorPos(&currentPoint))
            continue; 

        int16_t deltaX = static_cast<int16_t>(currentPoint.x - previousPoint.x);
        int16_t deltaY = static_cast<int16_t>(currentPoint.y - previousPoint.y);

        if (deltaX != 0 || deltaY != 0) {
            UsbPacket packet{};
            packet.command = CMD_MOUSE_REPORT;
            packet.data.mouse.dx = deltaX;
            packet.data.mouse.dy = deltaY;

            writer.Write(packet);
        }

        previousPoint = currentPoint;
        
    }
}