#include <gpio.h>
#include <usart.h>
#include <tim.h>
#include "app.h"
#include "log.h"
#include "md_adc.h"
#include "md_pwm.h"
#include "Scope.h"

static void setLed(bool enable) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void init(void) {
}

void setup(void) {
    LOGD("setup...");
    adcInit();
    pwmInit();

    static uint8_t data;
    HAL_UART_Receive_IT(&huart1, &data, 1);

    auto& scope = Scope::getInstance();
    scope.updateVolMax(3300);
    scope.setMcuImpl(
            {
                    .sendData = [](uint8_t* data, size_t size) {
                        HAL_UART_Transmit(&huart1, data, size, 0xFFFF);
                    },
                    .startSample = []{
                        HAL_TIM_Base_Start(&htim3);
                        setLed(true);
                    },
                    .stopSample = []{
                        HAL_TIM_Base_Stop(&htim3);
                        setLed(false);
#if 0
                        auto& message = Scope::getInstance().getMessage();
                        LOG("ch1: %d", message.sampleCh1[0]);
#endif
                    },
                    .setSampleFs = [](uint32_t fs) {
                        return adcSetFrequency(fs);
                    },
            });
}

void loop(void) {
    HAL_Delay(100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    uint8_t data = huart->Instance->DR;
    Scope::getInstance().onRead(&data, 1);
}
