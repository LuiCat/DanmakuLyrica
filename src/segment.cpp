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

void Segment::getEntityNotes(list<Note>& noteList)
{
    MapState tempState=segmentState;

    int prevNum=0;

    auto event_iter=events.begin();
    auto note_iter=notes.begin();

    Note tempNote;

    while(note_iter!=notes.end() || event_iter!=events.end())
    {
        while(event_iter!=events.end() && (note_iter==notes.end() || event_iter->num<=note_iter->num))
        {
            processEvent(&tempState, &*event_iter, event_iter->num-prevNum);
            prevNum=event_iter->num;

            ++event_iter;
        }

        if(note_iter==notes.end())
            continue;

        processEvent(&tempState, 0, note_iter->num-prevNum);
        prevNum=note_iter->num;

        tempNote.setJudgeTime(tempState.timeOffset, tempState.beatOffset);
        tempNote.setScrollSpeed(tempState.hs);
        tempNote.setNoteType(note_iter->type);

        noteList.push_back(tempNote);

        ++note_iter;
    }

}

void Segment::processEvent(MapState *state, const SegmentEvent *event, int deltaNum) const
{
    state->timeOffset+=state->calcOffset_segment((double)deltaNum/segmentDiv);
    state->beatOffset+=state->getTotalBeats()*((double)deltaNum/segmentDiv);
    state->processEvent(event);
}

void Segment::processEvent(MapState *state, const SegmentEvent *event, double deltaOffset) const
{
    state->timeOffset+=deltaOffset;
    state->beatOffset+=state->calcBeatOffset(deltaOffset);
    state->processEvent(event);
}
