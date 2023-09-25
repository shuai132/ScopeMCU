#include "md_adc.h"

#include <cmath>

#include "adc.h"
#include "log.h"
#include "tim.h"

volatile uint16_t ADCValue[CHANNEL_NUM];

void adc_init() {
    // 自动校准
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
        LOGF();
    }
}

void adc_start() {
    LOGD("adc_start");
    // start adc dma
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCValue, CHANNEL_NUM) != HAL_OK) {
        LOGF();
    }

    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        LOGF();
    }

    // start timer
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        LOGF();
    }
}

void adc_stop() {
    LOGD("adc_stop");
    // start timer
    if (HAL_TIM_Base_Stop_IT(&htim3) != HAL_OK) {
        LOGF();
    }
}

uint32_t adc_setFrequency(uint32_t frequency) {
    const uint32_t CLOCKS = HAL_RCC_GetSysClockFreq();
    const auto SYS_MHZ = CLOCKS / 1000000;
    uint32_t period = CLOCKS / frequency;

    uint16_t p1 = 0;
    uint16_t p2 = 0;
    {
        uint16_t pm = sqrt(period);
        do {
            if (pm == 1) {
                p1 = SYS_MHZ;
                p2 = 1000000 / frequency;
                break;
            }
            if (period % pm == 0) {
                p1 = pm;
                if (p1 * SYS_MHZ > 0xFFFF) continue;
                p2 = period / pm;
                break;
            }
        } while (--pm);
    }
    htim3.Init.Prescaler = p1 - 1;
    htim3.Init.Period = p2 - 1;

    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        LOGF();
    }

    uint32_t realFs = CLOCKS / (p1 * p2);
    LOGD("realFs: %ld, p1=%u, p2=%u", realFs, p1, p2);
    return realFs;
}

/**
 * STM32F0每颗芯片有个基于某个电压及温度下的精确测定值: VREFINT_CAL
 * STM32F1里同样有内部基准电压，但它没有像F0一样每颗芯片有个精确测定值。
 * 其电压范围在1.16-1.26间，一般取1.2V。
 */
uint16_t adc_getVolmV(int ch) {
    assert_param(0 <= ch && ch < CHANNEL_NUM);
    return 1200 * ADCValue[ch] / ADCValue[CHANNEL_NUM - 1];
}
