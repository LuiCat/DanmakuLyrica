#ifndef SEGMENT_H
#define SEGMENT_H

#include "notedef.h"

#include <list>
using namespace std;

class Segment
{
public:
    Segment();

    void init(MapState *state=0);

    void setSegmentDiv(int div);

    void appendNote(const SegmentNote& note);
    void appendEvent(const SegmentEvent& event);

    MapState getNextSegmentState();
    void nextSegmentState();

protected:

    void processEvent(MapState* state, const SegmentEvent *event) const;
    void processEvent(MapState* state, const SegmentEvent *event, int deltaNum) const;
    void processEvent(MapState* state, const SegmentEvent *event, double deltaOffset) const;

private:
    int segmentDiv;
    MapState segmentState;

    list<SegmentNote> notes;
    list<SegmentEvent> events;

};

#endif // SEGMENT_H
