#pragma once
#include <stdint.h>

#pragma pack(push, 1) // No padding so the size stays the same between modules
enum DeviceCommand : uint8_t {
    CMD_KEYBOARD_REPORT = 0x01,
    CMD_MOUSE_REPORT = 0x02,
    CMD_SET_CONFIG = 0x03
};
enum MouseButton : uint8_t {
    LEFT = 1 << 0,
    RIGHT = 1 << 1,
    MIDDLE = 1 << 2
};
enum ModifierKey : uint8_t {
    LEFT_CONTROL = 1 << 0,
    LEFT_SHIFT = 1 << 1,
    LEFT_ALT = 1 << 2,
    LEFT_COMMAND = 1 << 3,
    RIGHT_CONTROL = 1 << 4,
    RIGHT_SHIFT = 1 << 5,
    RIGHT_ALT = 1 << 6,
    RIGHT_COMMAND = 1 << 7
};
struct MousePayload {
    uint8_t buttonMask;
    int16_t dx;
    int16_t dy;
    int8_t dWheel;
    uint8_t reserved[2];
};
struct KeyboardPayload {
    uint8_t modifierMask;
    uint8_t reserved;
    uint8_t keys[6];
};
struct ConfigPayload {
    //uint16_t scale = 2708;
    int8_t maxDelta = 127;
    uint8_t reserved[7];
};
union HidPayload {
    MousePayload mouse;
    KeyboardPayload keyboard;
    ConfigPayload config;
};
struct UsbPacket {
    DeviceCommand command;
    HidPayload data;
};
#pragma pack(pop)