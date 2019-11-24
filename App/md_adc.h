#pragma once

#include <cstdint>

// ADC值 三个采集通道加一个校准通道 由DMA更改
#define CHANNEL_NUM 4
extern volatile uint16_t ADCValue[CHANNEL_NUM];

void adcInit();

uint16_t getVolmV(int ch);
