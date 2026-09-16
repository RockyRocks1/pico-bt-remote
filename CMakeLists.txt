cmake_minimum_required(VERSION 3.27)

if(BUILD_PICO_TARGET)
    include(pico/pico_sdk_import.cmake)
endif()

project(pico-bt-remote)

if(BUILD_PICO_TARGET AND NOT TARGET pico_stdlib)
    pico_sdk_init()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(shared)

if(BUILD_PICO_TARGET)
    add_subdirectory(pico)
else()
    add_subdirectory(controller)
endif()