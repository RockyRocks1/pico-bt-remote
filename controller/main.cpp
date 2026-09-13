#include "UsbStreamWriter.hpp"
#include <iostream>

int main() {
    std::string portName = "COM3";
    uint32_t baudRate = 115200;
    UsbStreamWriter writer;
    if (!writer.Open(portName, baudRate)) {
        std::cerr << "Failed to open serial port" << std::endl;
        return -1;
    }
}