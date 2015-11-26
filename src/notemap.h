#ifndef NOTEMAP_H
#define NOTEMAP_H

#include "notedef.h"
#include "segment.h"
#include "mapstate.h"
#include "beattime.h"

#include "noteentity.h"

#include <list>
using namespace std;

class NoteMap
{
public:

    NoteMap();

    bool loadTjaFile(const char* filename);

    BeatTime offsetMapState(MapState &state, double deltaSec) const;
    BeatTime offsetMapStateSingle(MapState &state, double deltaSec) const;

    int getNoteInfo(list<NoteInfo> &infoList, int maxinum=-1);

    const MapState& getBeginState();
    MapState getBgmBeginState();

    const char* getWavFilename();

protected:

    char tjafile[100];
    char wavfile[100];
    char title[100];
    char subtitle[200];
    bool showSub;

    MapState beginState;

    vector<Segment> segments;

    vector<Segment>::iterator loadProc;

    bool reloadTjaFile();

private:

    MapState currentState;

};

#endif // NOTEMAP_H
