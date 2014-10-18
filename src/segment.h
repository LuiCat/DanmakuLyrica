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

    void getEntityNotes(list<Note*> &noteList);

protected:

    void processEvent(MapState* state, const SegmentEvent *event, int deltaNum) const;
    void processEvent(MapState* state, const SegmentEvent *event, double deltaOffset) const;

private:

    int segmentDiv;
    MapState segmentState;

    list<SegmentNote> notes;
    list<SegmentEvent> events;

};

#endif // SEGMENT_H
