#include "app.h"
#include "usart.h"
#include "md_adc.h"
#include "md_pwm.h"
#include "md_led.h"
#include "md_usb.h"
#include "Scope.h"
#include "log.h"

using namespace scope;

void init(void) {
    usb_plugged();
    std::set_new_handler([] {
        FATAL("out of memory");
    });
}

void setup(void) {
    LOGD("setup...");
    adc_init();
    pwm_init();

    auto& scope = Scope::getInstance();
    scope.setVolLimits(0, 3300);
    scope.setFsLimits(1, 67000);
    scope.setMcuImpl(
            {
                    .sendData = [](uint8_t* data, size_t size) {
                        usb_cdcSend(data, size);
                    },
                    .startADC = std::bind(adc_start),
                    .stopADC = std::bind(adc_stop),
                    .setSampleFs = [](uint32_t fs) {
                        return adc_setFrequency(fs);
                    },
                    .onSampling = [](bool sampling) {
                        led_setValue(sampling);
                    },
            });
}

void loop(void) {
    HAL_Delay(500);
    if (not Scope::getInstance().isSampling()) {
        led_toggle();
    }
}

void on_usb_cdc_data(uint8_t* data, size_t size) {
    Scope::getInstance().onRead(data, size);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    Scope::getInstance().onADC(adc_getVolmV(0));
}
