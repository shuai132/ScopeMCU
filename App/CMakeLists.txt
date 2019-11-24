set(TARGET_NAME app)

add_library(${TARGET_NAME} app.cpp md_adc.cpp)
target_include_directories(${TARGET_NAME} PUBLIC .)
