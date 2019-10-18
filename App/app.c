#include <gpio.h>
#include "app.h"
#include "log.h"
#include "md_adc.h"

void setup(void) {
    LOGD("setup...");

    adcInit();
}

void loop(void) {
    LOGI("adc: %u, %u, %u, %u", ADCValue[0], ADCValue[1], ADCValue[2], ADCValue[3]);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(1000);
}
