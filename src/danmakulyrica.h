#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

#include "notescene.h"
#include "sounds.h"
#include "keybinding.h"
#include "enemy.h"

#include "timeline.h"

#include "texturestrip.h"

#include "bulletscene.h"
#include "bulletscript.h"
#include "scenemanager.h"

class DanmakuLyrica : public MainGame
{
public:

    static DanmakuLyrica* instance;

    explicit DanmakuLyrica();

protected:

    void mainInit();
    void mainCleanup();
    void mainUpdate();
    void mainRender();

private:

    static double inputOffset;

    NoteMap noteMap;
    NoteScene noteScene;

    MapState mapState;

    StreamBuffer bgm;

    BulletScene bulletScene;

    SceneManager sceneManager;

    BulletScript script;

    double bgmTimeStamp;
    TimeLine timeLine;

    Score score;

    KeyBinding buttonA;
    KeyBinding buttonB;
    KeyBinding buttonPause;
    KeyBinding buttonSkip;

    KeyBinding buttonUp;
    KeyBinding buttonDown;
    KeyBinding buttonLeft;
    KeyBinding buttonRight;

    Texture tex;
    TextureStrip strip;

    RenderTexture bulletSurface;
    RenderTexture noteSurface;

};

#endif // DANMAKULYRICA_H
