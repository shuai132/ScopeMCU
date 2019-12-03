#pragma once

#include <functional>
#include <cstdint>

#include "Portable.h"
#include "PacketProcessor.h"

namespace scope {

class Scope {
    struct MCU {
        // 发送数据到上位机
        std::function<void(uint8_t* data, size_t size)> sendData;

        std::function<void()> startADC;
        std::function<void()> stopADC;

        // 设置期望采样率并返回实际采样率
        std::function<uint32_t(uint32_t sampleFs)> setSampleFs;

        // 可用于led指示灯
        std::function<void(bool sampling)> onSampling;
    };

private:
    Scope();

public:
    static Scope& getInstance();

public:
    /**
     * 设置MCU要实现的一些功能
     * @param mcu
     */
    void setMcuImpl(MCU mcu);

    /**
     * 每采样一次adc 计算出mV并调用此方法
     * @param volmV
     */
    void onADC(uint16_t volmV);

    /**
     * 当前实现下采样值范围mV
     * @param volMinmV
     * @param volMaxmV
     */
    void setVolLimits(uint16_t volMinmV, uint16_t volMaxmV);

    /**
     * 当前实现下采样率范围sps
     * @param fsMinSps
     * @param fsMaxSps
     */
    void setFsLimits(uint32_t fsMinSps, uint32_t fsMaxSps);

    /**
     * 接收到上位机数据时调用
     * @param data
     * @param size
     */
    void onRead(uint8_t* data, size_t size);

    bool isSampling() {
        return sampling_;
    }

private:
    void addADC(uint16_t volmV);

    void startSample();

    void stopSample();

    void onSampleFinish();

    void updateFs(uint32_t fs);

    void updateSampleNum(uint32_t num);

    void updateTriggerMode(TriggerMode mode);

    void updateTriggerSlope(TriggerSlope slope);

    void updateTriggerLevel(TriggerLevel level);

private:
    PacketProcessor processor_;

    Message message_;

    SampleInfo& sampleInfo_ = message_.sampleInfo;

    bool sampling_ = false;

    uint16_t samplePos_ = 0;

    MCU mcu_;
};

}
