#pragma once

#include <cstdint>

#define CHANNEL_NUM 2

// ADC值 采集通道加一个校准通道 由DMA更改
extern volatile uint16_t ADCValue[CHANNEL_NUM];

void adc_init();

uint32_t adc_setFrequency(uint32_t frequency);

uint16_t adc_getVolmV(int ch);
