#include "Scope.h"

Scope::Scope()
    : processor_(false) {
    processor_.setOnPacketHandle([this](const std::string& payload) {
        Cmd* cmd = (Cmd*)payload.data();
        Cmd::Data data = cmd->data;
        switch (cmd->type) {
            case Cmd::Type::NONE:
                break;
            case Cmd::Type::SET_SAMPLE_FS:
                updateFs(mcu_.setSampleFs(data.sampleFs));
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
                startNewSample();
                break;
        }
    });
}

Scope& Scope::getInstance() {
    static Scope instance;
    return instance;
}

void Scope::add(uint16_t volmV) {
    message_.sampleCh1[samplePos_] = volmV;
    if (++samplePos_ == sampleInfo_.sampleNum) {
        mcu_.stopSample();
        samplePos_ = 0;
        onSampleFinish();
    }

    // trigger logic
    static uint16_t lastVol = volmV;
    if (sampleInfo_.triggerMode == TriggerMode::NORMAL) {
        switch (sampleInfo_.triggerSlope) {
            case TriggerSlope::UP:
                if (lastVol < sampleInfo_.triggerLevel && sampleInfo_.triggerLevel < volmV) {
                    startNewSample();
                }
                break;
            case TriggerSlope::DOWN:
                if (lastVol > sampleInfo_.triggerLevel && sampleInfo_.triggerLevel > volmV) {
                    startNewSample();
                }
                break;
        }
    }
    lastVol = volmV;
}

void Scope::setMcuImpl(MCU mcu) {
    mcu_ = std::move(mcu);
    mcu_.setSampleFs(10000);
    mcu_.startSample();
}

void Scope::updateVolMax(uint32_t volMaxmV) {
    sampleInfo_.volMaxmV = volMaxmV;
}

void Scope::onRead(uint8_t* data, size_t size) {
    processor_.feed(data, size);
}

const Message& Scope::getMessage() {
    return message_;
}

void Scope::startNewSample() {
    samplePos_ = 0;
    mcu_.startSample();
}

void Scope::onSampleFinish() {
    processor_.packForeach((uint8_t*)&message_, sizeof(message_), [this](uint8_t* data, size_t size) {
        mcu_.sendData(data, size);
    });
    if (sampleInfo_.triggerMode == TriggerMode::ALWAYS) {
        mcu_.startSample();
    }
}

void Scope::updateFs(uint32_t fs) {
    sampleInfo_.sampleFs = fs;
}

void Scope::updateSampleNum(uint32_t num) {
    sampleInfo_.sampleNum = num;
}

void Scope::updateTriggerMode(TriggerMode mode) {
    sampleInfo_.triggerMode = mode;
    if (mode == TriggerMode::SOFTWARE) {
        startNewSample();
    }
}

void Scope::updateTriggerSlope(TriggerSlope slope) {
    sampleInfo_.triggerSlope = slope;
}

void Scope::updateTriggerLevel(TriggerLevel level) {
    sampleInfo_.triggerLevel = level;
}
