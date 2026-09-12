#pragma once
#include <stdint.h>

enum DeviceCommand : uint8_t {
    CMD_KEYBOARD_REPORT = 0x01,
    CMD_MOUSE_REPORT = 0x02
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
    int8_t relativeX;
    int8_t relativeY;
    int8_t relativeWheel;
};
struct KeyboardPayload {
    uint8_t modifierMask;
    uint8_t reserved;
    uint8_t keys[6];
};
#pragma pack(push, 1) // No padding so the size stays the same between modules
union HidPayload {
    MousePayload mouse;
    KeyboardPayload keyboard;
};
struct UsbPacket {
    uint8_t command;
    HidPayload data;
};
#pragma pack(pop)