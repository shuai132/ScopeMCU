#include "Scope.h"
#include "PacketProcessor.h"

Scope::Scope() = default;

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
    static PacketProcessor processor(false);
    processor.packForeach((uint8_t*)&message_, sizeof(message_), [this](uint8_t* data, size_t size) {
        mcu_.sendData(data, size);
    });
    if (triggerMode_ == TriggerMode::ALWAYS) {
        mcu_.startSample();
    }
}
