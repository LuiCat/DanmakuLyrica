#ifndef NOTESCENE_H
#define NOTESCENE_H

#include "notelist.h"
#include "notemap.h"
#include "scene.h"
#include "texturepiece.h"
#include "numbersprite.h"
#include "score.h"

class NoteScene : public Scene
{
public:

    NoteScene();

    void load();
    void unload();

    void update(rtime_t deltaTime);
    void render();

    void setNoteMap(NoteMap* m);
    void reloadNotes();

    JudgeResult judgeSingleNote(double timeSec);
    inline int getCombo() const
    {
        return combo;
    }

    void breakCombo();

    Note* getNextPendingNote();

    void setScoreObj(Score* score);

protected:

    NoteList noteList;
    NoteMap* noteMap;

    TexturePiece panelBg;
    TexturePiece panelIcon;

    TexturePiece track;
    TexturePiece trackLight;

    TexturePiece title;

    BeatTime beatTime;

    int combo;
    NumberSprite comboNum;

    Score* score;
    NumberSprite scoreNum;

};

#endif // NOTESCENE_H
