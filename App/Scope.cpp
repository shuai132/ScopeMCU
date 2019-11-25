#include "Scope.h"

Scope::Scope()
    : processor_(false) {
    processor_.setOnPacketHandle([this](const std::string& payload) {
        Cmd* cmd = (Cmd*)payload.data();
        switch (cmd->type) {
            case Cmd::Type::NONE:
                break;
            case Cmd::Type::SET_SAMPLE_FS:
                updateFs(mcu_.setSampleFs(cmd->data));
                break;
            case Cmd::Type::SET_SAMPLE_NUM:
                updateSampleNum(cmd->data > SAMPLE_NUM_MAX ? SAMPLE_NUM_MAX : cmd->data);
                break;
            case Cmd::Type::SET_TRIGGER_MODE:
                triggerMode_ = static_cast<TriggerMode>(cmd->data);
                break;
            case Cmd::Type::TRIGGER_SAMPLE:
                mcu_.startSample();
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
    if (++samplePos_ == message_.sampleNum) {
        mcu_.stopSample();
        samplePos_ = 0;
        onSampleFinish();
    }
}

void Scope::updateVolMax(uint32_t volMaxmV) {
    message_.volMaxmV = volMaxmV;
}

void Scope::updateFs(uint32_t fs) {
    message_.sampleFs = fs;
}

void Scope::updateSampleNum(uint32_t num) {
    message_.sampleNum = num;
}

void Scope::setMcuImpl(MCU mcu) {
    mcu_ = std::move(mcu);
    mcu_.setSampleFs(10000);
    mcu_.startSample();
}

void Scope::onSampleFinish() {
    processor_.packForeach((uint8_t*)&message_, sizeof(message_), [this](uint8_t* data, size_t size) {
        mcu_.sendData(data, size);
    });
    if (triggerMode_ == TriggerMode::ALWAYS) {
        mcu_.startSample();
    }
}

void Scope::onRead(uint8_t* data, size_t size) {
    processor_.feed(data, size);
}
