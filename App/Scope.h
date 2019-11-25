#pragma once

#include <functional>
#include <cstdint>

#include "Portable.h"
#include "PacketProcessor.h"

class Scope {
    struct MCU {
        std::function<void(uint8_t* data, size_t size)> sendData;

        std::function<void()> startSample;
        std::function<void()> stopSample;

        // return real fs
        std::function<uint32_t(uint32_t sampleFs)> setSampleFs;
    };

    enum class TriggerMode {
        ALWAYS,
        UP,
        DOWN,
        AUTO,
        HAND,
    };

private:
    Scope();

public:
    static Scope& getInstance();

public:
    void setMcuImpl(MCU mcu);

    void add(uint16_t volmV);

    void updateVolMax(uint32_t volMaxmV);

    void onRead(uint8_t* data, size_t size);

private:
    void onSampleFinish();

    void updateFs(uint32_t fs);

    void updateSampleNum(uint32_t num);

    const Message& getMessage() {
        return message_;
    }

private:
    PacketProcessor processor_;

    Message message_;

    uint16_t samplePos_ = 0;

    MCU mcu_;

    TriggerMode triggerMode_ = TriggerMode::ALWAYS;
};
