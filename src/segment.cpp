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

MapState Segment::getNextSegmentState() const
{
    MapState state=segmentState;
    int prevNum=0;

    for(auto iter=events.begin(); iter!=events.end(); ++iter)
    {
        state.processEvent(&*iter, iter->num-prevNum, segmentDiv);
        prevNum=iter->num;
    }

    state.processEvent(0, segmentDiv-prevNum, segmentDiv);
    state.currentSegment+=1;

    return std::move(state);
}

void Segment::nextSegmentState()
{
    segmentState=getNextSegmentState();
}

double Segment::offsetMapState(MapState &state, double deltaSec) const
{
    MapState tempState=segmentState;
    int prevNum=0;
    auto iter=events.cbegin();
    double deltaOffset;

    for(; iter!=events.cend()&&tempState.timeOffset<=state.timeOffset; ++iter)
    {
        tempState.processEvent(&*iter, iter->num-prevNum, segmentDiv);
        prevNum=iter->num;
    }

    deltaOffset=tempState.timeOffset-state.timeOffset;
    if(deltaOffset>deltaSec)
    {
        state.processEvent(0, deltaSec);
        return 0.0;
    }

    state=tempState;
    deltaSec-=deltaOffset;

    for(; iter!=events.cend(); ++iter)
    {
        deltaOffset=state.calcOffset_segment((double)(iter->num-prevNum)/segmentDiv);
        if(deltaOffset>deltaSec)
        {
            state.processEvent(0, deltaSec);
            return 0.0;
        }
        state.processEvent(&*iter, deltaOffset);
        deltaSec-=deltaOffset;
        prevNum=iter->num;
    }

    deltaOffset=state.calcOffset_segment((double)(segmentDiv-prevNum)/segmentDiv);
    if(deltaOffset>deltaSec)
    {
        state.processEvent(0, deltaSec);
        return 0.0;
    }

    state.processEvent(0, deltaOffset);
    state.currentSegment++;

    return deltaSec-deltaOffset;
}

double Segment::offsetMapStateSingleEvent(MapState &state, double deltaSec) const
{
    MapState tempState=segmentState;
    int prevNum=0;
    auto iter=events.cbegin();
    double deltaOffset;

    for(; iter!=events.cend()&&tempState.timeOffset<=state.timeOffset; ++iter)
    {
        tempState.processEvent(&*iter, iter->num-prevNum, segmentDiv);
        prevNum=iter->num;
    }

    deltaOffset=tempState.timeOffset-state.timeOffset;
    if(deltaOffset>deltaSec)
    {
        state.processEvent(0, deltaSec);
        return 0.0;
    }

    state=tempState;
    deltaSec-=deltaOffset;

    if(iter!=events.cend())
    {
        deltaOffset=state.calcOffset_segment((double)(iter->num-prevNum)/segmentDiv);
        if(deltaOffset>deltaSec)
        {
            state.processEvent(0, deltaSec);
            return 0.0;
        }
        state.processEvent(&*iter, deltaOffset);
        deltaSec-=deltaOffset;
        prevNum=iter->num;
        return deltaSec;
    }

    deltaOffset=state.calcOffset_segment((double)(segmentDiv-prevNum)/segmentDiv);
    if(deltaOffset>deltaSec)
    {
        state.processEvent(0, deltaSec);
        return 0.0;
    }

    state.processEvent(0, deltaOffset);
    state.currentSegment++;

    return deltaSec-deltaOffset;
}

void Segment::getNoteInfo(list<NoteInfo>& infoList)
{
    MapState tempState=segmentState;

    int prevNum=0;

    auto event_iter=events.begin();
    auto note_iter=notes.begin();

    NoteInfo t;

    while(note_iter!=notes.end() || event_iter!=events.end())
    {
        while(event_iter!=events.end() && (note_iter==notes.end() || event_iter->num<=note_iter->num))
        {
            tempState.processEvent(&*event_iter, event_iter->num-prevNum, segmentDiv);
            prevNum=event_iter->num;

            ++event_iter;
        }

        if(note_iter==notes.end())
            continue;

        tempState.processEvent(0, note_iter->num-prevNum, segmentDiv);
        prevNum=note_iter->num;

        t.offsetSec=tempState.timeOffset;
        t.offsetBeat=tempState.beatOffset;
        t.hs=tempState.hs;
        t.noteType=note_iter->type;

        infoList.push_back(t);

        ++note_iter;
    }

}
