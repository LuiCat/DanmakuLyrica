#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

#include "notescene.h"
#include "sounds.h"
#include "keybinding.h"

#include "timeline.h"

#include "texturestrip.h"

#include "bulletscene.h"
#include "bulletscript.h"

class DanmakuLyrica : public MainGame
{
public:

    static DanmakuLyrica* instance;

    DanmakuLyrica();

protected:

    void mainInit();
    void mainCleanup();
    void mainUpdate();
    void mainRender();

private:

    NoteMap noteMap;
    NoteScene noteScene;

    MapState mapState;

    StreamBuffer bgm;

    BulletScene bulletScene;

    BulletScript script;

    double bgmTimeStamp;
    TimeLine timeLine;

    KeyBinding buttonA;
    KeyBinding buttonB;
    KeyBinding buttonPause;
    KeyBinding buttonSkip;

    Texture tex;
    TextureStrip strip;

};

#endif // DANMAKULYRICA_H
