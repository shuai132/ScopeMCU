#include "gpio.h"

void led_setValue(bool enable) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void led_toggle() {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
}
