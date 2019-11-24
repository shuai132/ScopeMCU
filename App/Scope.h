#pragma once

#include <functional>
#include <cstdint>

#include "Portable.h"

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

    const Message& getMessage() {
        return message_;
    }

private:
    void onSampleFinish();

    void updateFs(uint32_t fs);

    void updateSampleNum(uint32_t num);

private:
    Message message_;

    uint16_t samplePos_ = 0;

    MCU mcu_;

    TriggerMode triggerMode_ = TriggerMode::ALWAYS;
};
