#pragma once

#include <cstdint>
#include "Scope.h"

#define CHANNEL_NUM 2

// ADC值 采集通道加一个校准通道 由DMA更改
extern volatile uint16_t ADCValue[CHANNEL_NUM];

void adcInit();

uint32_t adcSetFrequency(uint32_t frequency);

uint16_t getVolmV(int ch);
