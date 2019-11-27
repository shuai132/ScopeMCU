#include "app.h"
#include "usart.h"
#include "tim.h"
#include "md_adc.h"
#include "md_pwm.h"
#include "md_led.h"
#include "Scope.h"
#include "log.h"

void init(void) {
}

void setup(void) {
    LOGD("setup...");
    adc_init();
    pwm_init();

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
                        led_setValue(true);
                    },
                    .stopSample = []{
                        HAL_TIM_Base_Stop(&htim3);
                        led_setValue(false);
#if 0
                        auto& message = Scope::getInstance().getMessage();
                        LOG("ch1: %d", message.sampleCh1[0]);
#endif
                    },
                    .setSampleFs = [](uint32_t fs) {
                        return adc_setFrequency(fs);
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

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    Scope::getInstance().add(adc_getVolmV(0));
}
