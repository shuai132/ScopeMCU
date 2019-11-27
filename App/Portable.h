#pragma once

#include <cstdint>

const uint16_t SAMPLE_NUM_MAX = 1024 * 2;

#pragma pack(push, 1)

enum class TriggerMode : uint8_t {
    ALWAYS = 0,
    NORMAL,
    SOFTWARE,
};

enum class TriggerSlope : uint8_t {
    UP = 0,
    DOWN,
};

using TriggerLevel = uint16_t;

struct SampleInfo {
    uint16_t sampleNum;
    uint16_t volMaxmV;
    uint32_t sampleFs;
    TriggerMode triggerMode;
    TriggerSlope triggerSlope;
    TriggerLevel triggerLevel;
};

struct Message  {
    SampleInfo sampleInfo{};
    uint16_t sampleCh1[SAMPLE_NUM_MAX]{};
};

struct Cmd  {
    enum class Type : uint8_t {
        NONE = 0,
        SET_SAMPLE_FS,      // data: fs(Hz)
        SET_SAMPLE_NUM,     // data: num(point)
        SET_TRIGGER_MODE,   // data: mode(TriggerMode)
        SET_TRIGGER_SLOPE,  // data: mode(TriggerLevel)
        SET_TRIGGER_LEVEL,  // data: mode(mV)
        SOFTWARE_TRIGGER,   // no data
    };

    union Data {
        uint32_t sampleFs;
        uint32_t sampleNum;
        TriggerMode triggerMode;
        TriggerSlope triggerSlope;
        TriggerLevel triggerLevel;
    };

    Type type = Type::NONE;
    Data data{};
};

#pragma pack(pop)
