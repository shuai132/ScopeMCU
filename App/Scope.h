#pragma once

#include <functional>
#include <cstdint>
#include <memory>

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
        std::function<SampleFs_t(SampleFs_t sampleFs)> setSampleFs;

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
     * @param vol
     */
    void onADC(SampleVo_t vol);

    /**
     * 当前实现下采样值范围mV
     * @param volMin
     * @param volMax
     */
    void setVolLimits(SampleVo_t volMin, SampleVo_t volMax);

    /**
     * 当前实现下采样率范围sps
     * @param fsMinSps
     * @param fsMaxSps
     */
    void setFsLimits(SampleFs_t fsMinSps, SampleFs_t fsMaxSps);

    /**
     * 设置最大采样点 取决于RAM大小
     * @param sn
     */
    void setMaxSn(SampleSn_t sn);

    /**
     * 接收到上位机数据时调用
     * @param data
     * @param size
     */
    void onRead(uint8_t* data, size_t size);

    bool isSampling();


private:
    void addADC(SampleVo_t vol);

    void startSample();

    void stopSample();

    void onSampleFinish();

    void updateFs(SampleFs_t fs);

    void updateSampleNum(SampleSn_t sn);

    void updateTriggerMode(TriggerMode mode);

    void updateTriggerSlope(TriggerSlope slope);

    void updateTriggerLevel(TriggerLevel vol);

private:
    PacketProcessor processor_;

    SampleInfo sampleInfo_{};
    SampleSn_t& maxSampleNum_ = sampleInfo_.sampleNumMax;
    std::unique_ptr<Message> message_;

    bool sampling_ = false;

    uint16_t samplePos_ = 0;

    MCU mcu_;
};

}
