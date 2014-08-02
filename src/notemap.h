#ifndef NOTEMAP_H
#define NOTEMAP_H

#include "notedef.h"
#include "segment.h"

#include <list>
using namespace std;

class NoteMap
{
public:

    NoteMap();

    bool loadTjaFile(const char* filename);

protected:

    char tjafile[100];
    char wavfile[100];
    char title[100];
    char subtitle[200];
    bool showSub;

    MapState beginState;

    list<Segment> segments;

    bool reloadTjaFile();

private:

    MapState currentState;

};

#endif // NOTEMAP_H
