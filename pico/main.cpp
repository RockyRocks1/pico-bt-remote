#include "pico/stdio.h"
#include "BlePico.hpp"

int main() {
    stdio_init_all();

    if (cyw43_arch_init())
        return -1;

    BlePico* blePico = BlePico::GetInstance();
    blePico->Initialize();
    blePico->RunExecutionLoop();

    return 0;
}
