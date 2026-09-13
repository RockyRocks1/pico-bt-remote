#pragma once
#include "pico/stdio.h"
#include "pico/error.h"
#include "InputManager.hpp"

class UsbStreamListener {
public:
    UsbStreamListener() {}
    void PollStreamAndPipeTo(InputManager& inputManager);
private:
    UsbPacket m_incomingPacket{};
    size_t m_bytesRead = 0;
};