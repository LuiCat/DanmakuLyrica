#ifndef MAPSTATE_H
#define MAPSTATE_H

#include "notedef.h"
#include "beattime.h"

#include <list>
using namespace std;

struct MapState
{
    double timeOffset;
    double beatOffset;

    double bpm;
    double hs;

    int measureDiv;
    int measureNum;

    int currentSegment;

    MapState(double _offset=0.0, double _bpm=120.0);

    inline BeatTimeVec toBeatTime() const
    {
        return BeatTimeVec(timeOffset, beatOffset);
    }

    inline double calcOffset_segment(double deltaSegment) const
    {
        return calcOffset_beat(deltaSegment*getTotalBeats());
    }

    inline double calcOffset_beat(double deltaBeat) const
    {
        return deltaBeat/bpm*60;
    }

    inline double calcDeltaSegment_offset(double deltaOffset) const
    {
        return calcBeatOffset(deltaOffset)/getTotalBeats();
    }

    inline double calcDeltaSegment_beat(double deltaBeat) const
    {
        return deltaBeat/getTotalBeats();
    }

    inline double calcBeatOffset(double deltaOffset) const
    {
        return deltaOffset/60*bpm;
    }

    inline double getTotalBeats() const
    {
        return (double)measureNum*4/measureDiv;
    }

    void processEvent(const SegmentEvent *event);
    void processEvent(const SegmentEvent *event, int deltaNum, int segmentDiv);
    void processEvent(const SegmentEvent *event, double deltaOffset);

};

#endif // MAPSTATE_H
