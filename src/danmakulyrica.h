#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

#include "notescene.h"
#include "sounds.h"
#include "keybinding.h"
#include "enemy.h"

#include "smoothtimer.h"
#include "timemanager.h"
#include "notemapbeatprovider.h"

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

    StreamSound bgm;

    BulletScene bulletScene;

    SceneManager sceneManager;

    TimeManager timeManager;
    SoundTimeProvider timeProvider;
    NotemapBeatProvider beatProvider;

    BulletScript script;

    Score score;

    bool isPaused;

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
