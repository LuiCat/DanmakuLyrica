#include "danmakulyrica.h"

#include "graphics.h"
#include "soundregistry.h"

#include "mathhelper.h"
#include "commondef.h"
#include "debug.h"

#include <fstream>

DanmakuLyrica* DanmakuLyrica::instance = 0;

#if defined(FPS_VSYNC)
double DanmakuLyrica::inputOffset = 0.0075;
#elif defined(FPS_UNLIMITED)
double DanmakuLyrica::inputOffset = 0;
#else
double DanmakuLyrica::inputOffset = FPS_LIMIT_MSEC*0.0005;
#endif

DanmakuLyrica::DanmakuLyrica()
    :script(&bulletScene)
    ,buttonA(DIK_Z, 0)
    ,buttonB(DIK_X, 0)
    ,buttonPause(DIK_SPACE, 0)
    ,buttonSkip(DIK_S, 0)
    ,buttonUp(DIK_UP, 0)
    ,buttonDown(DIK_DOWN, 0)
    ,buttonLeft(DIK_LEFT, 0)
    ,buttonRight(DIK_RIGHT, 0)
    ,isPaused(true)
{
    instance=this;
}

void DanmakuLyrica::mainInit()
{
    SoundRegistry::createSound("hit0", false, "data/sound/lyrica_notehit0.wav", 1.0f);
    SoundRegistry::createSound("hit1", false, "data/sound/lyrica_notehit1.wav", 1.0f);
    SoundRegistry::createSound("tan1", false, "data/sound/se_tan01.wav", 1.0f);
    SoundRegistry::createSound("pop", false, "data/sound/pop.wav", 0.5f);
    SoundRegistry::createSound("miss", false, "data/sound/miss.wav", 0.8f);

    createTexture("data/tex.png", &tex);
    strip.setTexture(tex);
    strip.setStripPos(0, 0, 0.5, 0.5);
    strip.setMaxIndex(2, 2);

    string stagename;
    string tjaname;
    ifstream fin("data/stage/stage.txt");
    for (int i = 0; i < 1; ++i)
    {
        getline(fin, stagename);
        getline(fin, tjaname);
    }
    fin.close();

    stagename = "data/stage/" + stagename + "/";

    tjaname = stagename + tjaname;
    noteMap.loadTjaFile(tjaname.c_str());

    bgm.load((stagename + noteMap.getWavFilename()).c_str());
    bgm.setVolume(0.5f);

    timeProvider.setSound(&bgm);
    timeProvider.setSeekDelay(0.03);
    beatProvider.setNoteMap(&noteMap);

    timeManager.setTimeProvider(&timeProvider);
    timeManager.setBeatProvider(&beatProvider);

    noteScene.setNoteMap(&noteMap);

    noteScene.setScoreObj(&score);
    bulletScene.setScoreObj(&score);

    sceneManager.pushScene(&bulletScene);
    sceneManager.pushScene(&noteScene);

    bulletScene.getPlayer()->setNoteScene(&noteScene);
    noteScene.setPlayer(bulletScene.getPlayer());

    TexturePiece bg((stagename + "bg.png").c_str());
    noteScene.setBackgroundImage(bg);

    auto beginState = noteMap.getBgmBeginState();
    stagename = stagename + "main.lua";
    script.setTime(beginState.beatOffset);
    script.loadScriptFile(stagename.c_str());

    TexturePiece::loadPending();

}

void DanmakuLyrica::mainCleanup()
{
    sceneManager.clearAllScene();
    bgm.release();
}

void DanmakuLyrica::mainUpdate()
{
    JudgeResult judgeResult;

    // do player motion first
    int flagDir = Dir_None;
    if (buttonUp.isDown())
        flagDir ^= Dir_Up;
    if (buttonDown.isDown())
        flagDir ^= Dir_Down;
    if (buttonLeft.isDown())
        flagDir ^= Dir_Left;
    if (buttonRight.isDown())
        flagDir ^= Dir_Right;
    bulletScene.setPlayerMotion((PlayerDirection)flagDir);

    //1: update time manager
    //2: update scenes by single lua event
    //2.1: get elapsed time from lua events
    //2.2: update scenes
    //2.3: update lua events
    //2.4: if elapsed time remaining, repeat 2

    if (!isPaused)
    {
        bool continueUpdate = true;
        while (continueUpdate)
        {
            continueUpdate = timeManager.updateTime(true);
            BeatTime& beatTime = timeManager.beatTime;
            while (beatTime.getDeltaBeat() > M_DINFS)
            {
                double taskElapsed = beatTime.getDeltaBeat() - script.seekNextTask(beatTime.getDeltaBeat());
                BeatTime taskTime = beatTime.splitBeat(taskElapsed);
                //BeatTimeVec deltaDelta(taskTime.getDeltaTime(), taskTime.getDeltaBeat());
                sceneManager.updateScene(taskTime);
                beatTime.proceedBeat(taskElapsed);
                script.updateSingleTask(beatTime.getDeltaBeat());
            }
        }

        if (buttonA.isPushed())
        {
            double bgmTime = timeManager.beatTime.getTime() + inputOffset;
            judgeResult = noteScene.judgeSingleNote(bgmTime);
            if (judgeResult>Judge_Miss)
            {
                SOUND("hit1")->play(true);
                bulletScene.hitPlayerTargetSpirit();
            }
            else
            {
                SOUND("hit0")->play(true);
            }
        }

    }

    if (buttonSkip.isPushed())
    {
        double jumpTime = bulletScene.getJump();
        if (timeManager.beatTime.getTime() < jumpTime)
        {
            bgm.pause();
            timeManager.seekTime(jumpTime);
            bgm.play();
        }
    }

    if (buttonPause.isPushed())
    {
        if (isPaused)
        {
            isPaused = !isPaused;
            timeManager.seekTime(0);
            bgm.play();
        }
    }

}

void DanmakuLyrica::mainRender()
{
    // render scenes
    d2d.beginScene();
    d2d.resetMatrix();

    d3d.beginScene();
    d3d.resetMatrix();

    //d2d.pushMatrix();
    sceneManager.renderScene();
    //d2d.popMatrix();

    d3d.drawScene();
    d3d.endScene();

    d2d.drawScene();
    d2d.endScene();

    d2d.present();

}
