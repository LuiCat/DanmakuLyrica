#ifndef MAPSTATE_H
#define MAPSTATE_H

#include "notedef.h"

struct MapState
{
    double timeOffset;
    double beatOffset;

    double bpm;
    double hs;

    int measureDiv;
    int measureNum;

    MapState(double _offset=0.0, double _bpm=120.0);

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

};

#endif // MAPSTATE_H
