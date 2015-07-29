#include "mapstate.h"


MapState::MapState(double _offset, double _bpm)
    :timeOffset(_offset)
    ,beatOffset(0)
    ,bpm(_bpm)
    ,hs(1.0)
    ,measureDiv(4)
    ,measureNum(4)
    ,currentSegment(0)
{

}

void MapState::processEvent(const SegmentEvent *event)
{
    if(event==0)
        return;
    switch(event->type)
    {
    case measure:
        if(event->num==0)
        {
            measureDiv=event->parameter_i_2;
            measureNum=event->parameter_i_1;
        }
        break;
    case scroll:
        hs=event->parameter_d;
        break;
    case bpmchange:
        bpm=event->parameter_d;
        break;
    default:
        break;
    }
}

void MapState::processEvent(const SegmentEvent *event, int deltaNum, int segmentDiv)
{
    timeOffset+=calcOffset_segment((double)deltaNum/segmentDiv);
    beatOffset+=getTotalBeats()*((double)deltaNum/segmentDiv);
    processEvent(event);
}

void MapState::processEvent(const SegmentEvent *event, double deltaOffset)
{
    timeOffset+=deltaOffset;
    beatOffset+=calcBeatOffset(deltaOffset);
    processEvent(event);
}

