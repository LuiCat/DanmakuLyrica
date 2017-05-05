// 编码：Utf-8 without BOM
// Created by LuiCat

#include "timemanager.h"
#include "mathhelper.h"

TimeManager::TimeManager()
    : timeProvider(nullptr)
    , beatProvider(nullptr)
    , timeElapsed(0)
{
    
}

void TimeManager::setTimeProvider(TimeProvider * timeProvider)
{
    this->timeProvider = timeProvider;
}

void TimeManager::setBeatProvider(BeatProvider * beatProvider)
{
    this->beatProvider = beatProvider;
}

bool TimeManager::updateTime(bool noBreak)
{
    if (timeProvider == nullptr)
        return false;

    if (beatProvider->needSeekTime())
    {
        beatProvider->seekTime(beatTime.currTime);
    }

    if (timeElapsed <= M_DINFS)
        timeElapsed = timeProvider->timeElapsed();

    if (!isfinite(timeElapsed))
        timeElapsed = 0;

    while (timeElapsed > M_DINFS)
    {
        //beatTime.proceed();

        double remainTime = beatProvider->updateTime(timeElapsed);
        double newBeat = beatProvider->getBeat();

        beatTime.setOffset(beatTime.deltaTime + timeElapsed - remainTime, newBeat - beatTime.currBeat);

        timeElapsed = remainTime;

        if (!noBreak)
            return true;
    }

    return false;
}

void TimeManager::seekTime(double timeSec)
{
    beatProvider->seekTime(timeSec);
    timeProvider->seekTime(timeSec);
    beatTime.currTime = timeSec;
    beatTime.currBeat = beatProvider->getBeat();
    beatTime.setOffset(0, 0);
    timeProvider->timeElapsed(); // reset elapsed time
}
