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

    double getBeatOffset();

    const char* getBgmFilename();

    JudgeResult judgeSingleNote(double timeSec);

protected:

    NoteList noteList;
    NoteMap noteMap;

    void reloadNotes();

};

#endif // NOTESCENE_H
