cmake_minimum_required(VERSION 3.27)

project(pico-bt-remote-host C CXX)

add_library(${PROJECT_NAME} STATIC
    src/UsbStreamWriter.cpp
    src/VideoCapture.cpp
    src/utils/pixel/PixelModifier.cpp
)

set(OPENPNP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/openpnp-capture")

target_sources(${PROJECT_NAME} PRIVATE
    ${OPENPNP_DIR}/common/context.cpp
    ${OPENPNP_DIR}/common/libmain.cpp
    ${OPENPNP_DIR}/common/logging.cpp
    ${OPENPNP_DIR}/common/stream.cpp
)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        ${OPENPNP_DIR}/win/PlatformContext.cpp
        ${OPENPNP_DIR}/win/PlatformStream.cpp
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE strmiids ole32 oleaut32 uuid)
elseif(APPLE)
    target_sources(${PROJECT_NAME} PRIVATE
        ${OPENPNP_DIR}/mac/PlatformContext.mm
        ${OPENPNP_DIR}/mac/PlatformStream.mm
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework AVFoundation -framework CoreMedia -framework CoreVideo")
else()
    target_sources(${PROJECT_NAME} PRIVATE
        ${OPENPNP_DIR}/linux/PlatformContext.cpp
        ${OPENPNP_DIR}/linux/PlatformStream.cpp
    )
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC OPENPNPCAPTURE_STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
    ${OPENPNP_DIR}/include
    ${OPENPNP_DIR}/common
)
