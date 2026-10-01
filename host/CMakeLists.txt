cmake_minimum_required(VERSION 3.27)

project(pico-bt-remote-host C CXX)

set(OPENPNP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/openpnp-capture")

if(WIN32)
    set(OS_CAPTURE_SOURCES
        ${OPENPNP_DIR}/win/PlatformContext.cpp
        ${OPENPNP_DIR}/win/PlatformStream.cpp
    )
    set(OS_LIBS strmiids ole32 oleaut32 uuid)
elseif(APPLE)
    set(OS_CAPTURE_SOURCES
        ${OPENPNP_DIR}/mac/PlatformContext.mm
        ${OPENPNP_DIR}/mac/PlatformStream.mm
    )
    set(OS_LIBS "-framework AVFoundation -framework CoreMedia -framework CoreVideo")
else()
    set(OS_CAPTURE_SOURCES
        ${OPENPNP_DIR}/linux/PlatformContext.cpp
        ${OPENPNP_DIR}/linux/PlatformStream.cpp
    )
    set(OS_LIBS "")
endif()


add_library(${PROJECT_NAME} STATIC
    UsbStreamWriter.cpp
    VideoCapture.cpp
    ${OS_CAPTURE_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
    ${OPENPNP_DIR}/include
    ${OPENPNP_DIR}/common
)

if(OS_LIBS)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OS_LIBS})
endif()
