// 编码：Utf-8 without BOM
// Created by LuiCat

#include "notemapbeatprovider.h"

NotemapBeatProvider::NotemapBeatProvider(NoteMap* notemap)
    : notemap(notemap)
    , notemapChanged(true)
{
    //mapState = notemap->getBeginState();
}

void NotemapBeatProvider::setNoteMap(NoteMap * notemap)
{
    this->notemap = notemap;
    this->notemapChanged = true;
}

void NotemapBeatProvider::seekTime(double timeSec)
{
    mapState = notemap->getStateByOffset(timeSec);
    notemapChanged = false;
}

double NotemapBeatProvider::updateTime(double deltaSec)
{
    auto elapsed = notemap->offsetMapStateSingle(mapState, deltaSec);
    return deltaSec - elapsed.time;
}

double NotemapBeatProvider::getBeat()
{
    return mapState.beatOffset;
}

bool NotemapBeatProvider::needSeekTime()
{
    return notemapChanged;
}
