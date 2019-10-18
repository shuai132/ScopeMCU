#include <adc.h>
#include <tim.h>
#include "md_adc.h"
#include "log.h"

volatile uint16_t ADCValue[CHANNEL_NUL];

void adcInit(void) {
    const int ChannelNum = 4;

    // 自动校准
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
        FATAL();
    }

    // start adc dma
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADCValue, ChannelNum) != HAL_OK) {
        FATAL();
    }

    // 设置采样频率: 1kHz
    htim3.Init.Prescaler = 72 - 1;
    htim3.Init.Period = 1000 - 1;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        FATAL();
    }

    // start timer
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        FATAL();
    }
}

/****************** weak callback ******************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    //LOGD("HAL_ADC_ConvCpltCallback");
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //LOGD("HAL_TIM_PeriodElapsedCallback");
}
