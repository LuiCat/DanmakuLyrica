#include "beattime.h"

const BeatTimeVec BeatTimeVec::zero(0, 0);

BeatTime::BeatTime(double time, double beat, double dTime, double dBeat)
    : currTime(time)
    , currBeat(beat)
    , deltaTime(dTime)
    , deltaBeat(dBeat)
{

}

BeatTime::BeatTime(double timeSec, double beat)
    : currTime(timeSec)
    , currBeat(beat)
    , deltaTime(0)
    , deltaBeat(0)
{

}

BeatTime::BeatTime(const BeatTime & other)
    : currTime(other.currTime)
    , currBeat(other.currBeat)
    , deltaTime(other.deltaTime)
    , deltaBeat(other.deltaBeat)
{

}

void BeatTime::addOffset(double offsetTime, double offsetBeat)
{
    deltaTime += offsetTime;
    deltaBeat += offsetBeat;
}

void BeatTime::setOffset(double offsetTime, double offsetBeat)
{
    deltaTime = offsetTime;
    deltaBeat = offsetBeat;
}

void BeatTime::setTarget(double targetTime, double targetBeat)
{
    deltaTime = targetTime - currTime;
    deltaBeat = targetBeat - currBeat;
}

void BeatTime::proceedTime(double time)
{
    double beat;
    if (deltaTime == 0)
    {
        if (deltaBeat == 0)
            return;
        time = 0;
        beat = deltaBeat;
        deltaBeat = 0;
    }
    else if(time >= deltaTime)
    {
        time = deltaTime;
        beat = deltaBeat;
        deltaTime = 0;
        deltaBeat = 0;
    }
    else
    {
        beat = deltaBeat * time / deltaTime;
        deltaTime -= time;
        deltaBeat -= beat;
    }
    currTime += time;
    currBeat += beat;
}

void BeatTime::proceedBeat(double beat)
{
    double time;
    if (deltaBeat == 0)
    {
        if (deltaTime == 0)
            return;
        beat = 0;
        time = deltaTime;
        deltaTime = 0;
    }
    else if (beat >= deltaBeat)
    {
        time = deltaTime;
        beat = deltaBeat;
        deltaTime = 0;
        deltaBeat = 0;
    }
    else
    {
        time = deltaTime * beat / deltaBeat;
        deltaTime -= time;
        deltaBeat -= beat;
    }
    currTime += time;
    currBeat += beat;
}

void BeatTime::proceed()
{
    currTime += deltaTime;
    currBeat += deltaBeat;
    deltaTime = 0;
    deltaBeat = 0;
}

BeatTime BeatTime::splitTime(double offsetTime)
{
    double offsetBeat;
    if (offsetTime >= deltaTime)
    {
        offsetTime = deltaTime;
        offsetBeat = deltaBeat;
    }
    else
    {
        offsetBeat = deltaBeat * offsetTime / deltaTime;
    }
    return BeatTime(currTime, currBeat, offsetTime, offsetBeat);
}

BeatTime BeatTime::splitBeat(double offsetBeat)
{
    double offsetTime;
    if (offsetBeat >= deltaBeat)
    {
        offsetTime = deltaTime;
        offsetBeat = deltaBeat;
    }
    else
    {
        offsetTime = deltaTime * offsetBeat / deltaBeat;
    }
    return BeatTime(currTime, currBeat, offsetTime, offsetBeat);
}

BeatTime BeatTime::split()
{
    return BeatTime(*this);
}
