#include "ticking.h"

Ticking::Ticking()
    :isDead(false)
    ,tick(0)
    ,timeSec(0.0)
    ,tickRate(60.0)
    ,nextTickSec(0.0)
    ,useDefaultTicking(true)
{
}

void Ticking::update(double deltaSec)
{
    if(isDead||tickRate<0.01)return;
    if(useDefaultTicking)
    {
        double newSec=timeSec+deltaSec;
        while(nextTickSec<=newSec)
        {
            ++tick;
            onUpdateMotion(nextTickSec-timeSec, (nextTickSec-timeSec)*tickRate);
            timeSec=nextTickSec;
            nextTickSec+=1.0/tickRate;
            onTick();
        }
        onUpdateMotion(newSec-timeSec, (newSec-timeSec)*tickRate);
        timeSec=newSec;
    }
    else
    {
        ++tick;
        onUpdateMotion(deltaSec, deltaSec*tickRate);
        onTick();
        timeSec+=deltaSec;
    }
}

void Ticking::setDead()
{
    isDead=true;
}

bool Ticking::dead() const
{
    return isDead;
}

int Ticking::getTick() const
{
    return tick;
}

double Ticking::getSec() const
{
    return timeSec;
}

void Ticking::onTick() {}

void Ticking::onUpdateMotion(double, double) {}

