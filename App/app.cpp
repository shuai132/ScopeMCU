#include <gpio.h>
#include <usart.h>
#include <tim.h>
#include "app.h"
#include "log.h"
#include "md_adc.h"
#include "md_pwm.h"
#include "PacketProcessor.h"
#include "PortableMCU.h"

static PacketProcessor processor(false); // NOLINT(cert-err58-cpp)

static void setLed(bool enable) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void init(void) {
}

void setup(void) {
    LOGD("setup...");
    adcInit();
    pwmInit();
}

void loop(void) {
    using namespace G;
    if (sampleOk) {
        setLed(true);
#if 1
        auto p = processor.pack((const uint8_t*) &message, sizeof(message));
        HAL_UART_Transmit(&huart1, (uint8_t*)p.data(), p.size(), 0xFFFF);
#else
        LOG("ch1: %d", message.sampleCh1[0]);
#endif
        setLed(false);
        sampleOk = false;
        HAL_TIM_Base_Start_IT(&htim3);
    }
}
