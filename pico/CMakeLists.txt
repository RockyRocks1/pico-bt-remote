cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../out/pico")
set(PICO_BOARD pico2_w CACHE STRING "Board type")

set(PICO_SDK_FETCH_FROM_GIT ON CACHE BOOL "Fetch SDK from git")
set(PICO_SDK_VERSION_STRING "2.1.1" CACHE STRING "Pico SDK version")

include(pico_sdk_import.cmake)

project(pico-bt-remote C CXX ASM)
set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.cpp
    BlePico.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    pico_cyw43_arch_none
    pico_btstack_ble
    pico_btstack_cyw43
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    CYW43_ENABLE_BLUETOOTH=1
)
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})
pico_btstack_make_gatt_header(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/pico_remote.gatt")
