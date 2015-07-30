#ifndef NOTESCENE_H
#define NOTESCENE_H

#include "notelist.h"
#include "notemap.h"
#include "scene.h"

class NoteScene : public Scene
{
public:

    NoteScene();

    void load();
    void unload();

    void update(double deltaSec);
    void render();

    void setNoteMap(NoteMap* m);
    void reloadNotes();

    JudgeResult judgeSingleNote(double timeSec);

protected:

    NoteList noteList;
    NoteMap* noteMap;

};

#endif // NOTESCENE_H
