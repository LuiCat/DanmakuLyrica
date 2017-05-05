#include "timeprovider.h"
// 编码：Utf-8 without BOM
// Created by LuiCat

#include "debug.h"

SoundTimeProvider::SoundTimeProvider()
    : sound(nullptr)
    , delay(0)
{
    timer.reset();
}

void SoundTimeProvider::setSound(Sound* newBgm)
{
    timer.reset();
    double offset = 0;
    if (sound != nullptr)
        offset = sound->getTime() - soundTimeStamp;
    sound = newBgm;
    if (sound != nullptr)
        soundTimeStamp = sound->getTime() - offset;
}

void SoundTimeProvider::setSoundTime(double timeSec)
{
    sound->setTime(timeSec - delay);
    timer.reset();
    soundTimeStamp = getSoundTime();
}

double SoundTimeProvider::getSoundTime()
{
    return sound->getTime() + delay;
}

void SoundTimeProvider::setSeekDelay(double delay)
{
    this->delay = delay;
}

void SoundTimeProvider::seekTime(double timeSec)
{
    setSoundTime(timeSec);
}

double SoundTimeProvider::timeElapsed()
{
    if (sound == nullptr || !sound->isPlaying())
    {
        return timer.getDeltaTime();
    }
    double bgmTime = getSoundTime();
    double result = timer.getDeltaTimeFixed(bgmTime - soundTimeStamp);
    soundTimeStamp = bgmTime;
    return result;
}
