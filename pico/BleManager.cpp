#include "BleManager.hpp"

static const uint8_t hidReportDescriptor[] = {
    0x05, 0x01,         //  USAGE_PAGE (Generic Desktop)
    0x09, 0x02,         //  USAGE (Mouse)
    0xa1, 0x01,         //  COLLECTION (Application)
    0x85, 0x01,         //      Report ID 1
    0x09, 0x01,         //      USAGE (Pointer)
    0xa1, 0x00,         //      COLLECTION (Physical)
    0x05, 0x09,         //          USAGE_PAGE (Button)
    0x19, 0x01,         //          USAGE_MINIMUM (Button 1)
    0x29, 0x03,         //          USAGE_MAXIMUM (Button 3)
    0x15, 0x00,         //          LOGICAL_MINIMUM (0)
    0x25, 0x01,         //          LOGICAL_MAXIMUM (1)
    0x95, 0x03,         //          REPORT_COUNT (3)
    0x75, 0x01,         //          REPORT_SIZE (1)
    0x81, 0x02,         //          INPUT (Data,Var,Abs)
    0x95, 0x01,         //          REPORT_COUNT (1)
    0x75, 0x05,         //          REPORT_SIZE (5)
    0x81, 0x03,         //          INPUT (Cnst,Var,Abs)
    0x05, 0x01,         //          USAGE_PAGE (Generic Desktop)
    0x09, 0x30,         //          USAGE (X)
    0x09, 0x31,         //          USAGE (Y)
    0x09, 0x38,         //          USAGE (WHEEL)
    0x15, 0x81,         //          LOGICAL_MINIMUM (-127)
    0x25, 0x7f,         //          LOGICAL_MAXIMUM (127)
    0x75, 0x08,         //          REPORT_SIZE (8)
    0x95, 0x02,         //          REPORT_COUNT (3)
    0x81, 0x06,         //          INPUT (Data,Var,Rel)
    0xc0,               //      END_COLLECTION
    0xc0,               //  END_COLLECTION
    0x05, 0x01,         //  USAGE_PAGE (Generic Desktop)
    0x09, 0x06,         //  USAGE (Keyboard)
    0xa1, 0x01,         //  COLLECTION (Application)
    0x85, 0x02,         //      Report ID 2
    0x75, 0x01,         //      REPORT_SIZE (1)
    0x95, 0x08,         //      REPORT_COUNT (8)
    0x05, 0x07,         //      USAGE_PAGE (Key codes)
    0x19, 0xe0,         //      USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,         //      USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,         //      LOGICAL_MINIMUM (0)
    0x25, 0x01,         //      LOGICAL_MAXIMUM (1)
    0x81, 0x02,         //      INPUT (Data,Var,Abs)
    0x75, 0x01,         //      REPORT_SIZE (1)
    0x95, 0x08,         //      REPORT_COUNT (8)
    0x81, 0x03,         //      INPUT (Cnst,Var,Abs)
    0x95, 0x05,         //      REPORT_COUNT (5)
    0x75, 0x01,         //      REPORT_SIZE (1)
    0x05, 0x08,         //      USAGE_PAGE (LEDs)
    0x19, 0x01,         //      USAGE_MINIMUM (Num Lock)
    0x29, 0x05,         //      USAGE_MAXIMUM (Kana)
    0x91, 0x02,         //      OUTPUT (Data,Var,Abs)
    0x95, 0x01,         //      REPORT_COUNT (1)
    0x75, 0x03,         //      REPORT_SIZE (3)
    0x91, 0x03,         //      OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,         //      REPORT_COUNT (6)
    0x75, 0x08,         //      REPORT_SIZE (8)
    0x15, 0x00,         //      LOGICAL_MINIMUM (0)
    0x25, 0xff,         //      LOGICAL_MAXIMUM (255)
    0x05, 0x07,         //      USAGE_PAGE (Key codes)
    0x19, 0x00,         //      USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xff,         //      USAGE_MAXIMUM (Reserved)
    0x81, 0x00,         //      INPUT (Data, Array)
    0xc0,               //  END_COLLECTION

};

// also temporary
static btstack_packet_callback_registration_t hci_event_callback_registration;
static btstack_packet_callback_registration_t sm_event_callback_registration;

BleManager* BleManager::instance = nullptr;
BleManager* BleManager::GetInstance() {
    if (!BleManager::instance)
        BleManager::instance = new BleManager();
    return BleManager::instance;
}

void BleManager::Initialize() {
    btstack_memory_init();
    btstack_run_loop_init(btstack_run_loop_async_context_get_instance(cyw43_arch_async_context()));

    l2cap_init();
    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
    sm_set_authentication_requirements(SM_AUTHREQ_SECURE_CONNECTION | SM_AUTHREQ_BONDING);

    att_server_init(profile_data, NULL, NULL);
    device_information_service_server_init();
    hids_device_init(0, hidReportDescriptor, sizeof(hidReportDescriptor));

    hci_event_callback_registration.callback = &BleManager::PacketHandler;
    hci_add_event_handler(&hci_event_callback_registration);

    sm_event_callback_registration.callback = &BleManager::PacketHandler;
    sm_add_event_handler(&sm_event_callback_registration);

    hids_device_register_packet_handler(&BleManager::PacketHandler);
    hci_power_control(HCI_POWER_ON);
    m_canSend = true;
}
void BleManager::SendMouseReport(MousePayload& mouse) {
    if (m_hidCid == HCI_CON_HANDLE_INVALID)
        return;

    uint8_t report[5] = {
        0x01,
        mouse.buttonMask,
        static_cast<uint8_t>(mouse.dx),
        static_cast<uint8_t>(mouse.dy),
        static_cast<uint8_t>(mouse.dWheel)
    };
    hids_device_send_input_report(m_hidCid, report, sizeof(report));
    m_canSend = false;
}
void BleManager::SendKeyboardReport(KeyboardPayload& keyboard) {
    if (m_hidCid == HCI_CON_HANDLE_INVALID)
        return;

    uint8_t report[9] = {
        0x02,
        keyboard.modifierMask,
        keyboard.reserved,
        keyboard.keys[0],
        keyboard.keys[1],
        keyboard.keys[2],
        keyboard.keys[3],
        keyboard.keys[4],
        keyboard.keys[5]
    };

    hids_device_send_input_report(m_hidCid, report, sizeof(report));
}
void BleManager::SendReportFromPacket(UsbPacket& packet) {
    if (m_hidCid == HCI_CON_HANDLE_INVALID)
        return;
    switch (packet.command) {
    case CMD_KEYBOARD_REPORT:
        SendKeyboardReport(packet.data.keyboard);
    case CMD_MOUSE_REPORT:
        SendMouseReport(packet.data.mouse);
    default:
        break;
    }
}
void BleManager::PacketHandler(uint8_t packetType, uint16_t channel, uint8_t* packet, uint16_t size) {
    GetInstance()->HandleBluetoothEvent(packetType, packet);
}
void BleManager::HandleBluetoothEvent(uint8_t packetType, uint8_t* packet) {
    if (packetType != HCI_EVENT_PACKET)
        return;
    if (hci_event_packet_get_type(packet) == BTSTACK_EVENT_STATE) {
        if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
            StartAdvertising();
            return;
        }
    }
    switch (hci_event_packet_get_type(packet)) {
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        m_hidCid = HCI_CON_HANDLE_INVALID;
        StartAdvertising();
        break;
    case SM_EVENT_JUST_WORKS_REQUEST:
        sm_just_works_confirm(sm_event_just_works_request_get_handle(packet));
        break;
    case SM_EVENT_NUMERIC_COMPARISON_REQUEST:
        sm_numeric_comparison_confirm(sm_event_passkey_display_number_get_handle(packet));
        break;
    case HCI_EVENT_HIDS_META:
        switch (hci_event_hids_meta_get_subevent_code(packet)) {
        case HIDS_SUBEVENT_CAN_SEND_NOW: {
            std::optional<UsbPacket> packet = m_inputManager->GetNextPacket();
            if (!packet)
                break;
            SendReportFromPacket(*packet);
            m_canSend = true;
            break;
        }
        case HIDS_SUBEVENT_INPUT_REPORT_ENABLE:
        case HIDS_SUBEVENT_BOOT_KEYBOARD_INPUT_REPORT_ENABLE:
        case HIDS_SUBEVENT_BOOT_MOUSE_INPUT_REPORT_ENABLE:
            m_hidCid = hids_subevent_input_report_enable_get_con_handle(packet);
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }
}

void BleManager::StartAdvertising() {
    static const uint8_t advertisingData[] = {
        0x02, BLUETOOTH_DATA_TYPE_FLAGS, 0x06,
        0x0C, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'P', 'i', 'c', 'o', ' ', 'R', 'e', 'm', 'o', 't', 'e',
        0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, ORG_BLUETOOTH_SERVICE_HUMAN_INTERFACE_DEVICE & 0xFF, ORG_BLUETOOTH_SERVICE_HUMAN_INTERFACE_DEVICE >> 8,
        0x03, BLUETOOTH_DATA_TYPE_APPEARANCE, 0xC2, 0x02
    };
    bd_addr_t nullAddress;
    memset(nullAddress, 0, 6);
    gap_advertisements_set_params(0x0030, 0x0030, 0, 0, nullAddress, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(advertisingData), const_cast<uint8_t*>(advertisingData));
    gap_advertisements_enable(1);
}

void BleManager::PollBluetooth() {
    cyw43_bluetooth_hci_process();
    cyw43_arch_poll();
    
    if (!m_inputManager->IsPacketQueueEmpty() && m_canSend && m_hidCid != HCI_CON_HANDLE_INVALID) {
        m_canSend = false;
        hids_device_request_can_send_now_event(m_hidCid);
    }
}
