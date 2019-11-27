#include "gpio.h"

void led_setValue(bool enable) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}
