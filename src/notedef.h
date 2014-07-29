#ifndef NOTEDEF_H
#define NOTEDEF_H

struct SegmentNote
{
    int num;
    enum NoteType {none, type1, type2, type3, type4}
        type;
};

struct SegmentEvent
{
    int num;
    enum EventType {unknown, bpmchange,
                    measure, scroll}
                       type;
    int parameter_i_1, parameter_i_2;
    double parameter_d;
};

struct MapState
{
    double offset;
    double beatOffset;

    double bpm;
    double scroll;

    int measureDiv;
    int measureNum;

    MapState(double _offset=0.0, double _bpm=120.0) :
        offset(_offset),
        beatOffset(0),
        bpm(_bpm),
        scroll(1.0),
        measureDiv(4),
        measureNum(4)
    {

    }

    inline double calcOffset_segment(double deltaSegment)
    {
        return calcOffset_beat(deltaSegment*getTotalBeats());
    }

    inline double calcOffset_beat(double deltaBeat)
    {
        return deltaBeat/bpm*60;
    }

    inline double calcDeltaSegment_offset(double deltaOffset)
    {
        return calcBeatOffset(deltaOffset)/getTotalBeats();
    }

    inline double calcDeltaSegment_beat(double deltaBeat)
    {
        return deltaBeat/getTotalBeats();
    }

    inline double calcBeatOffset(double deltaOffset)
    {
        return deltaOffset/60*bpm;
    }

    inline double getTotalBeats()
    {
        return (double)measureNum*4/measureDiv;
    }

};

#endif // NOTEDEF_H
