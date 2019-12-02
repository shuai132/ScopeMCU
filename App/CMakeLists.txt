set(TARGET_NAME app)

# PacketProcessor
option(packetprocessor_BUILD_EXAMPLE "build example" OFF)
add_subdirectory(modules/PacketProcessor)
link_libraries(packetprocessor)
target_compile_definitions(packetprocessor PRIVATE NDEBUG)

# scope define
add_definitions(-DSCOPE_IS_MCU)

add_library(${TARGET_NAME} app.cpp md_adc.cpp md_pwm.cpp md_led.cpp md_usb.cpp Scope.cpp)
target_include_directories(${TARGET_NAME} PUBLIC .)
