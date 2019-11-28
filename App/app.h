#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

void init(void);

void setup(void);

void loop(void);

void on_usb_cdc_data(uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif
