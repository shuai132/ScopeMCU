#pragma once

#include <cstdint>

namespace scope {

#if SCOPE_IS_MCU
const uint16_t SAMPLE_NUM_MAX = 1024 * 2;
#endif

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
    // device info
    uint16_t volMinmV;
    uint16_t volMaxmV;
    uint32_t fsMinSps;
    uint32_t fsMaxSps;
    uint32_t sampleNumMax
#if SCOPE_IS_MCU
            = SAMPLE_NUM_MAX;
#else
            ;
#endif

    // sample info
    uint16_t sampleNum;
    uint32_t sampleFs;
    TriggerMode triggerMode;
    TriggerSlope triggerSlope;
    TriggerLevel triggerLevel;
};

struct Message  {
    SampleInfo sampleInfo{};
#if SCOPE_IS_MCU
    uint16_t sampleCh1[SAMPLE_NUM_MAX]{};
#elif SCOPE_IS_GUI
    uint16_t sampleCh1[0];
#endif

    uint16_t getSizeNow() {
        return sizeof(sampleInfo) + sizeof(uint16_t) * sampleInfo.sampleNum;
    }
};

struct Cmd  {
    enum class Type : uint8_t {
        NONE = 0,
        SET_SAMPLE_FS,      // data: fs(Hz)
        SET_SAMPLE_NUM,     // data: num(point)
        SET_TRIGGER_MODE,   // data: mode(TriggerMode)
        SET_TRIGGER_SLOPE,  // data: slope(TriggerSlope)
        SET_TRIGGER_LEVEL,  // data: level(TriggerLevel)
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

}
