#ifndef NOTEMAP_H
#define NOTEMAP_H

#include "notedef.h"
#include "segment.h"
#include "mapstate.h"

#include "noteentity.h"

#include <list>
using namespace std;

class NoteMap
{
public:

    NoteMap();

    bool loadTjaFile(const char* filename);

    void offsetMapState(MapState &state, double deltaSec) const;

    int getEntityNotes(list<Note*> &noteList, int maxinum=-1);

    const char* getWavFilename();

protected:

    char tjafile[100];
    char wavfile[100];
    char title[100];
    char subtitle[200];
    bool showSub;

    MapState beginState;

    list<Segment> segments;

    list<Segment>::iterator loadProc;

    bool reloadTjaFile();

private:

    MapState currentState;

};

#endif // NOTEMAP_H
