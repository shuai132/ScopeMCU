#pragma once
#include <cstdint>

void usb_plugged();

void usb_cdcSend(uint8_t* data, size_t size);
