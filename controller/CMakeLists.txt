cmake_minimum_required(VERSION 3.27)

project(pico-bt-remote-controller C CXX)

add_executable(${PROJECT_NAME}
    UsbStreamWriter.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)