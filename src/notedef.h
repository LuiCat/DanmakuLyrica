#ifndef NOTEDEF_H
#define NOTEDEF_H

enum NoteType
{
    none,
    note1,
    note2,
    note3,
    note4
};

enum EventType
{
    unknown,
    bpmchange,
    measure,
    scroll
};

struct SegmentNote
{
    int num;
    NoteType type;
};

struct SegmentEvent
{
    int num;
    EventType type;
    int parameter_i_1, parameter_i_2;
    double parameter_d;
};

struct NoteInfo
{
    double offsetSec;
    double offsetBeat;
    double hs;
    NoteType noteType;
};

class NoteMap;
class Segment;
struct MapState;

#endif // NOTEDEF_H
