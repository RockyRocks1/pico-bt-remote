#include "pico/stdio.h"
#include "ConfigManager.hpp"
#include "BleManager.hpp"
#include "InputManager.hpp"
#include "UsbStreamListener.hpp"

int main() {
    stdio_init_all();

    if (cyw43_arch_init())
        return -1;

    ConfigManager configManager;
    InputManager inputManager(&configManager);
    UsbStreamListener usbStreamListener;
    BleManager* bleManager = BleManager::GetInstance();

    bleManager->SetInputManager(&inputManager);
    bleManager->Initialize();

    absolute_time_t nextBlinkTime = make_timeout_time_ms(2000);
    bool toggleBlink = true;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    while (true) {
        usbStreamListener.PollStreamAndPipeTo(inputManager);
        bleManager->PollBluetooth();

        if (absolute_time_diff_us(get_absolute_time(), nextBlinkTime) < 0) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, toggleBlink);
            toggleBlink = !toggleBlink;
            nextBlinkTime = make_timeout_time_ms(1000);
        }
    }


    return 0;
}
