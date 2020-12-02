#include "app.h"
#include "usart.h"
#include "md_adc.h"
#include "md_pwm.h"
#include "md_led.h"
#include "md_usb.h"
#include "ScopeMCU.h"
#include "log.h"

using namespace scope;

static const size_t MaxSn = 4096;
static uint8_t Buffer[Message::CalcBytes(MaxSn)];
static ScopeMCU scopeMcu(MaxSn, Buffer);

void init(void) {
    usb_plugged();
    std::set_new_handler([] {
        LOGW("out of memory");
        for(;;){
            led_toggle();
            HAL_Delay(50);
        }
    });
}

void setup(void) {
    LOGD("setup...");
    adc_init();
    pwm_init();

    scopeMcu.setVolLimits(0, 3300);
    scopeMcu.setFsLimits(1, 70000);
    scopeMcu.setMcuImpl(
            {
                    .sendData = [](uint8_t* data, size_t size) {
                        usb_cdcSend(data, size);
                    },
                    .startADC = std::bind(adc_start),
                    .stopADC = std::bind(adc_stop),
                    .setSampleFs = [](SampleFs_t fs) {
                        return adc_setFrequency(fs);
                    },
                    .onSampling = [](bool sampling) {
                        led_setValue(sampling);
                    },
            });
}

void loop(void) {
    HAL_Delay(500);
    if (not scopeMcu.isSampling()) {
        led_toggle();
    }
}

void on_usb_cdc_data(uint8_t* data, size_t size) {
    scopeMcu.onRead(data, size);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    scopeMcu.onADC(adc_getVolmV(0));
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
//}
