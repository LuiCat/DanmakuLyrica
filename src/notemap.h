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

    BeatTimeVec offsetMapState(MapState &state, double deltaSec) const;
    BeatTimeVec offsetMapStateSingle(MapState &state, double deltaSec) const;

    int getNoteInfo(list<NoteInfo> &infoList, int maxinum=-1);

    const MapState& getBeginState();
    MapState getBgmBeginState();
    MapState getStateByOffset(double offset);

    const string& getWavFilename();

protected:

    string tjafile;
    string wavfile;
    string title;
    string subtitle;
    bool showSub;

    MapState beginState;

    vector<Segment> segments;

    vector<Segment>::iterator loadProc;

    bool reloadTjaFile();

private:

    MapState currentState;

};

#endif // NOTEMAP_H
