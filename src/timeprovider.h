// 编码：Utf-8 without BOM
// Created by LuiCat

#ifndef _TIME_PROVIDER_H_
#define _TIME_PROVIDER_H_

#include "sound/sounds.h"
#include "smoothtimer.h"

class TimeProvider
{
public:
    virtual void seekTime(double timeSec) = 0;
    virtual double timeElapsed() = 0;
};

class SoundTimeProvider : public TimeProvider
{
private:

    SmoothTimer timer;

    Sound* sound;
    double soundTimeStamp;

    double delay;

public:

    SoundTimeProvider();

    void setSound(Sound* newBgm);

    void setSoundTime(double timeSec);
    double getSoundTime();

    void setSeekDelay(double delay);

    void seekTime(double timeSec) override;
    double timeElapsed() override;
};

#endif // _TIME_PROVIDER_H_
