#include "md_pwm.h"

#include "log.h"
#include "tim.h"

void pwm_init() {
    // 设置PWM频率: 1kHz
    const unsigned int SYS_MHZ = HAL_RCC_GetSysClockFreq() / 1000000;
    htim1.Init.Prescaler = SYS_MHZ - 1;
    htim1.Init.Period = 1000 - 1;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
        LOGF();
    }

    htim1.Instance->CCR1 = 500 - 1;
    // pwm start
    if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) {
        LOGF();
    }
}
