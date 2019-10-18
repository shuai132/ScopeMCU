set(TARGET_NAME app)

add_library(${TARGET_NAME} app.c retarget.c md_adc.c)
target_include_directories(${TARGET_NAME} PUBLIC .)

add_custom_target(build_and_flash cd ${CMAKE_BINARY_DIR} && make && st-flash write ${PROJECT_NAME}.bin 0x8000000)
