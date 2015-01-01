#include "ticking.h"

#include "mathhelper.h"

Ticking::Ticking(double tickRate)
    :isDead(false)
    ,tick(0)
    ,timeSec(0.0)
    ,useDefaultTicking(true)
    ,processSec(0.0)
{
    setTickRate(tickRate);
}

void Ticking::update(double deltaSec)
{
    if(isDead)return;
    if(useDefaultTicking)
    {
        double newSec=timeSec+deltaSec;

        while(timeSec+processSec<newSec)
        {
            onUpdateMotion(processSec, processSec*tickRate);
            ++tick;
            timeSec+=processSec;
            onTick();
            if(isDead)return;
            processSec=tickSec;
        }
        deltaSec=newSec-timeSec;
        onUpdateMotion(deltaSec, deltaSec*tickRate);
        timeSec=newSec;
        processSec-=deltaSec;
    }
    else
    {
        onUpdateMotion(deltaSec, deltaSec*tickRate);
        ++tick;
        timeSec+=deltaSec;
        onTick();
    }
}

double Ticking::singleTick(double deltaSec)
{
    if(isDead)return deltaSec;
    if(processSec<deltaSec)
    {
        onUpdateMotion(processSec, processSec*tickRate);
        ++tick;
        timeSec+=processSec;
        onTick();
        deltaSec-=processSec;
        if(!isDead)
            processSec=tickSec;
        return deltaSec;
    }
    onUpdateMotion(deltaSec, deltaSec*tickRate);
    processSec-=deltaSec;
    timeSec+=deltaSec;
    return 0.0;
}

void Ticking::setDead()
{
    isDead=true;
}

bool Ticking::dead() const
{
    return isDead;
}

void Ticking::reset(bool tickZero)
{
    tick=0;
    timeSec=0.0;
    processSec=(tickZero?0.0:tickSec);
}

void Ticking::setTickRate(double ticksPerSec)
{
    if(ticksPerSec>100.0)ticksPerSec=100.0;
    if(ticksPerSec<M_INFS)
    {
        tickRate=0;
        tickSec=M_INF;
    }
    else
    {
        tickRate=ticksPerSec;
        tickSec=1.0/tickRate;
    }
}

void Ticking::setTickSec(double secsPerTick)
{
    if(secsPerTick<0.01)secsPerTick=0.01;
    tickSec=secsPerTick;
    tickRate=1.0/tickSec;
}

int Ticking::getTick() const
{
    return tick;
}

double Ticking::getTimeSec() const
{
    return timeSec;
}

void Ticking::onTick() {}

void Ticking::onUpdateMotion(double, double) {}

