#pragma once

#include <cstdint>

const uint16_t SAMPLE_NUM_MAX = 1024 * 2;

#pragma pack(push, 2)
struct Message  {
    uint32_t volMaxmV  = 0;
    uint32_t sampleFs  = 0;
    uint16_t sampleNum = SAMPLE_NUM_MAX;
    uint16_t sampleCh1[SAMPLE_NUM_MAX]{};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Cmd  {
    enum class Type : uint8_t {
        NONE = 0,
        SET_SAMPLE_FS,      // data: fs
        SET_SAMPLE_NUM,     // data: num
        SET_TRIGGER_MODE,   // data: mode
        TRIGGER_SAMPLE,     // no data
    };

    enum class TriggerMode {
        ALWAYS = 0,
        UP,
        DOWN,
        AUTO,
        HAND,
    };

    Type type = Type::NONE;
    uint32_t data{};
};
#pragma pack(pop)
