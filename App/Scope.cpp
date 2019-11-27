#include "Scope.h"

Scope::Scope()
    : processor_(false) {
    processor_.setOnPacketHandle([this](uint8_t* payload, size_t size) {
        stopSample();
        Cmd* cmd = (Cmd*)payload;
        Cmd::Data data = cmd->data;
        switch (cmd->type) {
            case Cmd::Type::NONE:
                break;
            case Cmd::Type::SET_SAMPLE_FS:
                updateFs(data.sampleFs);
                break;
            case Cmd::Type::SET_SAMPLE_NUM:
                updateSampleNum(data.sampleNum > SAMPLE_NUM_MAX ? SAMPLE_NUM_MAX : data.sampleNum);
                break;
            case Cmd::Type::SET_TRIGGER_MODE:
                updateTriggerMode(data.triggerMode);
                break;
            case Cmd::Type::SET_TRIGGER_SLOPE:
                updateTriggerSlope(data.triggerSlope);
                break;
            case Cmd::Type::SET_TRIGGER_LEVEL:
                updateTriggerLevel(data.triggerLevel);
                break;
            case Cmd::Type::SOFTWARE_TRIGGER:
                startSample();
                break;
        }
    });
}

Scope& Scope::getInstance() {
    static Scope instance;
    return instance;
}

void Scope::setMcuImpl(MCU mcu) {
    mcu_ = std::move(mcu);
    updateFs(10000);
    updateSampleNum(1024);
    updateTriggerMode(TriggerMode::NORMAL);
    updateTriggerSlope(TriggerSlope::UP);
    updateTriggerLevel(1000);
    startSample();
    mcu_.startADC();
}

void Scope::onADC(uint16_t volmV) {
    static uint16_t lastVol = volmV;

    if (sampling_) {
        addADC(volmV);
        return;
    }

    // trigger logic
    if (sampleInfo_.triggerMode == TriggerMode::NORMAL) {
        switch (sampleInfo_.triggerSlope) {
            case TriggerSlope::UP:
                if (lastVol < sampleInfo_.triggerLevel && sampleInfo_.triggerLevel < volmV) {
                    startSample();
                    addADC(lastVol);
                    addADC(volmV);
                }
                break;
            case TriggerSlope::DOWN:
                if (lastVol > sampleInfo_.triggerLevel && sampleInfo_.triggerLevel > volmV) {
                    startSample();
                    addADC(lastVol);
                    addADC(volmV);
                }
                break;
        }
    }
    else if (sampleInfo_.triggerMode == TriggerMode::ALWAYS) {
        startSample();
    }
    lastVol = volmV;
}

void Scope::setVolMax(uint32_t volMaxmV) {
    sampleInfo_.volMaxmV = volMaxmV;
}

void Scope::onRead(uint8_t* data, size_t size) {
    processor_.feed(data, size);
}

void Scope::addADC(uint16_t volmV) {
    message_.sampleCh1[samplePos_] = volmV;
    if (++samplePos_ >= sampleInfo_.sampleNum) {
        onSampleFinish();
    }
};

const Message& Scope::getMessage() {
    return message_;
}

void Scope::startSample() {
    sampling_ = true;
    samplePos_ = 0;
    mcu_.setSampling(true);
}

void Scope::stopSample() {
    sampling_ = false;
    samplePos_ = 0;
    mcu_.setSampling(false);
}

void Scope::onSampleFinish() {
    stopSample();
#if 1
    processor_.packForeach((uint8_t*)&message_, sizeof(message_), [this](uint8_t* data, size_t size) {
        mcu_.sendData(data, size);
    });
#else
    auto& message = Scope::getInstance().getMessage();
    printf("ch1: %d\r\n", message.sampleCh1[0]);
#endif
    if (sampleInfo_.triggerMode == TriggerMode::ALWAYS) {
        startSample();
    }
}

void Scope::updateFs(uint32_t fs) {
    auto realFs = mcu_.setSampleFs(fs);
    sampleInfo_.sampleFs = realFs;
}

void Scope::updateSampleNum(uint32_t num) {
    if (num > SAMPLE_NUM_MAX) {
        num = SAMPLE_NUM_MAX;
    }
    sampleInfo_.sampleNum = num;
}

void Scope::updateTriggerMode(TriggerMode mode) {
    sampleInfo_.triggerMode = mode;
    if (mode == TriggerMode::SOFTWARE) {
        startSample();
    }
}

void Scope::updateTriggerSlope(TriggerSlope slope) {
    sampleInfo_.triggerSlope = slope;
}

void Scope::updateTriggerLevel(TriggerLevel level) {
    sampleInfo_.triggerLevel = level;
}
