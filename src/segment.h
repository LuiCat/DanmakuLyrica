#ifndef SEGMENT_H
#define SEGMENT_H

#include "notedef.h"
#include "mapstate.h"

#include <list>
using namespace std;

#include "noteentity.h"

class Segment
{
public:

    Segment();

    void init(MapState *state=0);

    void setSegmentDiv(int div);

    void appendNote(const SegmentNote& note);
    void appendEvent(const SegmentEvent& event);

    MapState getNextSegmentState() const;
    void nextSegmentState();

    double offsetMapState(MapState& state, double deltaSec) const;
    double offsetMapStateSingleEvent(MapState& state, double deltaSec) const;

    void getNoteInfo(list<NoteInfo>& infoList);

private:

    int segmentDiv;
    MapState segmentState;

    vector<SegmentNote> notes;
    vector<SegmentEvent> events;

};

#endif // SEGMENT_H
