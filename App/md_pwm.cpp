#include "md_pwm.h"
#include "tim.h"
#include "log.h"

void pwm_init() {
    // 设置PWM频率: 1kHz: 48 000 / 48 = 1000
    htim2.Init.Prescaler = 48 - 1;
    htim2.Init.Period = 1000 - 1;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        FATAL();
    }

    htim2.Instance->CCR3 = 500 - 1;
    // pwm start
    if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK) {
        FATAL();
    }
}
