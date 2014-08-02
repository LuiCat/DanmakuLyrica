#ifndef NOTESCENE_H
#define NOTESCENE_H

#include "notelist.h"
#include "notemap.h"

class NoteScene
{
public:

    NoteScene();

    void init();
    void cleanup();

    void update(double deltaSec);
    void render();

protected:

    NoteList noteList;
    NoteMap noteMap;

};

#endif // NOTESCENE_H
