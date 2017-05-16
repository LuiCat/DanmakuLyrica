#ifndef NOTESCENE_H
#define NOTESCENE_H

#include "notelist.h"
#include "notemap.h"
#include "scene.h"
#include "texturepiece.h"
#include "numbersprite.h"
#include "score.h"
#include "noterenderer.h"

class NoteScene : public Scene
{
public:

    NoteScene();

    void load();
    void unload();

    void update();
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

    void setPlayer(Entity* player);

    void setBackgroundImage(const TexturePiece& bg);

protected:

    list<NoteInfo> noteInfoList;

    NoteList noteList;
    NoteMap* noteMap;

    TexturePiece panelBg;
    TexturePiece panelIcon;

    TexturePiece track;
    TexturePiece trackLight;

    TexturePiece title;

    TexturePiece bg;

    BeatTimeVec beatTime;

    int combo;
    NumberSprite comboNum;

    Score* score;
    NumberSprite scoreNum;

    NoteRenderer noteRenderer;

    Entity* entityPlayer;
    double lerpPlayerX;
    double lerpPlayerY;

};

#endif // NOTESCENE_H
