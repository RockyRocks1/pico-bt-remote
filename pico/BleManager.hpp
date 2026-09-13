#pragma once
#include <cstdarg>
#include <cstdint>
#include <optional>

#include "pico/stdio.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_run_loop_async_context.h"
#include "ble/gatt-service/hids_device.h" 
#include "ble/gatt-service/battery_service_server.h"
#include "ble/gatt-service/device_information_service_server.h"
#include "pico_remote.h"

#include "shared/shared.h"
#include "InputManager.hpp"

class BleManager {
public:
    static BleManager* GetInstance();
	void Initialize();
    void SendReportFromPacket(UsbPacket& packet);
    void PollBluetooth();
    inline void SetInputManager(InputManager* inputManager) {
        m_inputManager = inputManager;
    }
private:
    BleManager() {};
    static BleManager* instance;
    static void PacketHandler(uint8_t packetType, uint16_t channel, uint8_t* packet, uint16_t size);
    bool m_canSend = false;
    uint16_t m_hidCid = HCI_CON_HANDLE_INVALID;
    void SendMouseReport(MousePayload& mouse);
    void SendKeyboardReport(KeyboardPayload& keyboard);
    void HandleBluetoothEvent(uint8_t packetType, uint8_t* packet);
    void StartAdvertising();
    InputManager* m_inputManager;
};