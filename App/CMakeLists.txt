set(TARGET_NAME app)

# PacketProcessor
add_subdirectory(modules/PacketProcessor)
link_libraries(packetprocessor)

add_library(${TARGET_NAME} app.cpp md_adc.cpp md_pwm.cpp PortableMCU.cpp)
target_include_directories(${TARGET_NAME} PUBLIC .)
