#include "md_pwm.h"
#include "tim.h"
#include "log.h"

void pwm_init() {
    // 设置PWM频率: 1kHz: 72 000 / 72 = 1000
    htim1.Init.Prescaler = 72;
    htim1.Init.Period = 1000 - 1;
    if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
        FATAL();
    }

    htim1.Instance->CCR1 = 500 - 1;
    // pwm start
    if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK) {
        FATAL();
    }
}
