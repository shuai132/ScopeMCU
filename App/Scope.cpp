#include "Scope.h"
#include "log.h"

namespace scope {

Scope::Scope()
    : processor_(false) {
    processor_.setMaxBufferSize(sizeof(Cmd));
    processor_.setOnPacketHandle([this](uint8_t* payload, size_t size) {
        stopSample();
        Cmd* cmd = (Cmd*)payload;
        Cmd::Data data = cmd->data;
        switch (cmd->type) {
            case Cmd::Type::NONE:
                break;
            case Cmd::Type::SET_SAMPLE_FS:
                updateFs(data.sampleFs);
                break;
            case Cmd::Type::SET_SAMPLE_NUM:
                updateSampleNum(data.sampleNum > maxSampleNum_ ? maxSampleNum_ : data.sampleNum);
                break;
            case Cmd::Type::SET_TRIGGER_MODE:
                updateTriggerMode(data.triggerMode);
                break;
            case Cmd::Type::SET_TRIGGER_SLOPE:
                updateTriggerSlope(data.triggerSlope);
                break;
            case Cmd::Type::SET_TRIGGER_LEVEL:
                updateTriggerLevel(data.triggerLevel);
                break;
            case Cmd::Type::SOFTWARE_TRIGGER:
                startSample();
                break;
        }
    });
}

Scope& Scope::getInstance() {
    static Scope instance;
    return instance;
}

void Scope::setMcuImpl(MCU mcu) {
    mcu_ = std::move(mcu);
    updateFs(50000);
    updateSampleNum(512);
    updateTriggerMode(TriggerMode::NORMAL);
    updateTriggerSlope(TriggerSlope::UP);
    updateTriggerLevel(1000);
    mcu_.startADC();
}

void Scope::onADC(SampleVo_t vol) {
    static auto lastVol = vol;

    if (sampling_) {
        addADC(vol);
        return;
    }

    // trigger logic
    if (sampleInfo_.triggerMode == TriggerMode::NORMAL) {
        switch (sampleInfo_.triggerSlope) {
            case TriggerSlope::UP:
                if (lastVol < sampleInfo_.triggerLevel && sampleInfo_.triggerLevel < vol) {
                    startSample();
                    addADC(lastVol);
                    addADC(vol);
                }
                break;
            case TriggerSlope::DOWN:
                if (lastVol > sampleInfo_.triggerLevel && sampleInfo_.triggerLevel > vol) {
                    startSample();
                    addADC(lastVol);
                    addADC(vol);
                }
                break;
        }
    }
    else if (sampleInfo_.triggerMode == TriggerMode::ALWAYS) {
        startSample();
    }
    lastVol = vol;
}

void Scope::setVolLimits(SampleVo_t volMin, SampleVo_t volMax) {
    sampleInfo_.volMinmV = volMin;
    sampleInfo_.volMaxmV = volMax;
}

void Scope::setFsLimits(SampleFs_t fsMinSps, SampleFs_t fsMaxSps) {
    sampleInfo_.fsMinSps = fsMinSps;
    sampleInfo_.fsMaxSps = fsMaxSps;
}

void Scope::setMaxSn(SampleSn_t sn) {
    maxSampleNum_ = sn;
    message_.reset((Message*)new char[Message::CalcBytes(maxSampleNum_)]);
}

void Scope::onRead(uint8_t* data, size_t size) {
    processor_.feed(data, size);
}

bool Scope::isSampling() {
    return sampling_;
}

void Scope::addADC(SampleVo_t vol) {
    message_->sampleCh1[samplePos_] = vol;
    if (++samplePos_ >= sampleInfo_.sampleSn) {
        onSampleFinish();
    }
};

void Scope::startSample() {
    sampling_ = true;
    samplePos_ = 0;
    mcu_.onSampling(true);
}

void Scope::stopSample() {
    sampling_ = false;
    samplePos_ = 0;
    mcu_.onSampling(false);
}

void Scope::onSampleFinish() {
    stopSample();
    message_->sampleInfo = sampleInfo_;
    processor_.packForeach(message_.get(), Message::CalcBytes(sampleInfo_.sampleSn), [this](uint8_t* data, size_t size) {
        mcu_.sendData(data, size);
    });

    //LOGD("ch1: %d", message_->sampleCh1[0]);

    if (sampleInfo_.triggerMode == TriggerMode::ALWAYS) {
        startSample();
    }
}

void Scope::updateFs(SampleFs_t fs) {
    if (fs > sampleInfo_.fsMaxSps) {
        fs = sampleInfo_.fsMaxSps;
    }
    auto realFs = mcu_.setSampleFs(fs);
    sampleInfo_.sampleFs = realFs;
}

void Scope::updateSampleNum(SampleSn_t sn) {
    if (sn > maxSampleNum_) {
        sn = maxSampleNum_;
    } else if (sn < 2) {
        sn = 2;
    }
    sampleInfo_.sampleSn = sn;
}

void Scope::updateTriggerMode(TriggerMode mode) {
    sampleInfo_.triggerMode = mode;
    if (mode == TriggerMode::SOFTWARE) {
        startSample();
    }
}

void Scope::updateTriggerSlope(TriggerSlope slope) {
    sampleInfo_.triggerSlope = slope;
}

void Scope::updateTriggerLevel(TriggerLevel vol) {
    sampleInfo_.triggerLevel = vol;
}

}
