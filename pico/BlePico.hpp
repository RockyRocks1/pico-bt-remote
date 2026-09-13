#pragma once
#include "shared/shared.h"
#include "pico/stdio.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_run_loop_async_context.h"
#include "ble/gatt-service/hids_device.h" 
#include "ble/gatt-service/battery_service_server.h"
#include "ble/gatt-service/device_information_service_server.h"
#include "pico_remote.h"

class BlePico {
public:
    static BlePico* GetInstance();
    void Initialize();
    void RunExecutionLoop();
private:
    BlePico() {}
    static void PacketHandler(uint8_t packetType, uint16_t channel, uint8_t* packet, uint16_t size);
    void PollUsbStream();
    void SendMouseReport(MousePayload& mouse);
    void SendKeyboardReport(KeyboardPayload& keyboard);
    void StartAdvertising();
    inline bool CanSendReport() const noexcept {
        return m_hidCid != HCI_CON_HANDLE_INVALID && m_canSend;
    }
    void HandleBluetoothEvent(uint8_t packetType, uint8_t* packet);


    static BlePico* instance;
    uint16_t m_hidCid = HCI_CON_HANDLE_INVALID;
    bool m_canSend = false;
    std::vector<HidPayload> m_inputQueue = {};
};