#include "beatticking.h"

#include "mathhelper.h"

BeatTicking::BeatTicking(double _beatTickRate, double _secTickRate)
    :Ticking(_secTickRate)
    ,timeBeat(0.0)
{
    setBeatTickRate(_beatTickRate);
}

void BeatTicking::resetBeat(bool tickZero)
{
    timeBeat=0.0;
    processBeat=(tickZero?0.0:tickSec);
}

void BeatTicking::setBeatTickRate(double ticksPerBeat)
{
    if(ticksPerBeat>192.0)ticksPerBeat=192.0;
    if(ticksPerSec<M_DINFS)
    {
        beatTickRate=0;
        beatTickBeat=M_DINF;
    }
    else
    {
        beatTickRate=ticksPerSec;
        beatTickBeat=1.0/beatTickRate;
    }
}

void BeatTicking::setBeatTickBeat(double beatsPerTick)
{
    if(beatsPerTick<1/192.0)beatsPerTick=1/192.0;
    beatTickBeat=beatsPerTick;
    beatTickRate=1.0/beatTickBeat;
}

double BeatTicking::getTimeBeat() const
{
    return timeBeat;
}

void BeatTicking::updateBeat(double deltaBeat)
{
    if(isDead)return;
    if(useDefaultTicking)
    {
        double newBeat=timeBeat+deltaBeat;

        while(timeBeat+processBeat<newBeat)
        {
            onUpdateMotion(processBeat, processBeat*beatTickRate);
            ++tick;
            timeBeat+=processBeat;
            onTick();
            if(isDead)return;
            processBeat=beatTickBeat;
        }
        deltaBeat=newBeat-timeBeat;
        onUpdateMotion(deltaBeat, deltaBeat*beatTickRate);
        timeBeat=newBeat;
        processBeat-=deltaBeat;
        processSec=0;
    }
    else
    {
        onUpdateMotion(deltaBeat, deltaBeat*beatTickRate);
        ++tick;
        timeBeat+=deltaBeat;
        onTick();
    }
}

void BeatTicking::singleBeatTick()
{
    singleBeatTick(M_DINF);
}

double BeatTicking::singleBeatTick(double deltaBeat)
{
    if(isDead)return deltaBeat;
    if(processBeat<deltaBeat)
    {
        if(processBeat!=0.0)
            onUpdateMotion(processBeat, processBeat*beatTickRate);
        ++tick;
        timeBeat+=processBeat;
        onTick();
        deltaBeat-=processBeat;
        processSec=0.0;
        if(!isDead)
            processBeat=beatTickBeat;
        return deltaBeat;
    }
    onUpdateMotion(deltaBeat, deltaBeat*beatTickRate);
    timeBeat+=deltaBeat;
    processBeat-=deltaBeat;
    processSec=0.0;
    return 0.0;
}

void BeatTicking::seekNextBeatTick()
{
    seekNextBeatTick(M_DINF);
}

double BeatTicking::seekNextBeatTick(double deltaBeat)
{
    if(isDead)return deltaBeat;
    if(processBeat<deltaBeat)
    {
        onUpdateMotion(processBeat, processBeat*beatTickRate);
        ++tick;
        timeBeat+=processBeat;
        deltaBeat-=processBeat;
        processBeat=0.0;
        processSec=0.0;
        return deltaBeat;
    }
    onUpdateMotion(deltaBeat, deltaBeat*beatTickRate);
    timeBeat+=deltaBeat;
    processBeat-=deltaBeat;
    processSec=0.0;
    return 0.0;
}
