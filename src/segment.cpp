#include "segment.h"

Segment::Segment()
{
}

void Segment::init(MapState *state)
{
    notes.clear();
    events.clear();
    if(state!=0)
    {
        segmentState=*state;
    }
}

void Segment::setSegmentDiv(int div)
{
    segmentDiv=div;
}

void Segment::appendNote(const SegmentNote &note)
{
    notes.push_back(note);
}

void Segment::appendEvent(const SegmentEvent &event)
{
    events.push_back(event);
}

MapState Segment::getNextSegmentState()
{
    MapState state=segmentState;
    int prevNum=0;

    for(auto iter=events.begin(); iter!=events.end(); ++iter)
    {
        processEvent(&state, &*iter, iter->num-prevNum);
        prevNum=iter->num;
    }

    processEvent(&state, 0, segmentDiv-prevNum);

    return state;
}

void Segment::nextSegmentState()
{
    segmentState=getNextSegmentState();
}

void Segment::processEvent(MapState *state, const SegmentEvent *event) const
{
    if(event==0)
        return;
    switch(event->type)
    {
    case SegmentEvent::measure:
        if(event->num==0)
        {
            state->measureDiv=event->param_i_2;
            state->measureNum=event->param_i_1;
        }
        break;
    case SegmentEvent::scroll:
        state->scroll=event->param_d;
        break;
    case SegmentEvent::bpmchange:
        state->bpm=event->param_d;
        break;
    default:
        break;
    }
}

void Segment::processEvent(MapState *state, const SegmentEvent *event, int deltaNum) const
{
    state->offset+=state->calcOffset_segment((double)deltaNum/segmentDiv);
    state->beatOffset+=state->getTotalBeats()*((double)deltaNum/segmentDiv);
    processEvent(state, event);
}

void Segment::processEvent(MapState *state, const SegmentEvent *event, double deltaOffset) const
{
    state->offset+=deltaOffset;
    state->beatOffset+=state->calcBeatOffset(deltaOffset);
    processEvent(state, event);
}
