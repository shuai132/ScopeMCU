set(TARGET_NAME app)

# ScopeCore
add_subdirectory(modules/ScopeCore)
link_libraries(ScopeCoreMCU)

add_library(${TARGET_NAME} app.cpp md_adc.cpp md_pwm.cpp md_led.cpp md_usb.cpp)
target_include_directories(${TARGET_NAME} PUBLIC .)
