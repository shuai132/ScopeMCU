#pragma once

#include <cstdint>

const uint16_t SAMPLE_NUM_MAX = 1024 * 2;

#pragma pack(push, 2)
struct Message  {
    uint32_t volMax    = 3300;
    uint32_t sampleFs  = 0;
    uint16_t sampleNum = SAMPLE_NUM_MAX;
    uint16_t sampleCh1[SAMPLE_NUM_MAX]{};
};
#pragma pack(pop)
