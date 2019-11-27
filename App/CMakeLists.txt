set(TARGET_NAME app)

# PacketProcessor
option(packetprocessor_BUILD_EXAMPLE "build example" OFF)
add_subdirectory(modules/PacketProcessor)
link_libraries(packetprocessor)

add_library(${TARGET_NAME} app.cpp md_adc.cpp md_pwm.cpp md_led.cpp Scope.cpp)
target_include_directories(${TARGET_NAME} PUBLIC .)
