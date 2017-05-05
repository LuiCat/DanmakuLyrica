// 编码：Utf-8 without BOM
// Created by LuiCat

#include "beatprovider.h"

bool BeatProvider::needSeekTime()
{
    return false;
}

LinearBeat::LinearBeat(double offset, double bpm)
    : currBeat(0)
    , beatPerSec(bpm / 60)
    , offset(offset)
    , bpmChanged(true)
{

}

void LinearBeat::setBpmOffset(double offset, double bpm)
{
    this->offset = offset;
    this->beatPerSec = bpm / 60;
}

void LinearBeat::seekTime(double timeSec)
{
    currTime = timeSec;
    currBeat = (timeSec - offset) * beatPerSec;
    bpmChanged = false;
}

double LinearBeat::updateTime(double deltaSec)
{
    currTime += deltaSec;
    currBeat += deltaSec * beatPerSec;
    return 0;
}

double LinearBeat::getBeat()
{
    return currBeat;
}

bool LinearBeat::needSeekTime()
{
    return bpmChanged;
}
