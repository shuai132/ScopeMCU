#include "gpio.h"

void led_setValue(bool enable) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void led_toggle() {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
}
