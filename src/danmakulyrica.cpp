#include "danmakulyrica.h"

#include "graphics.h"
#include "soundregistry.h"

#include "mathhelper.h"
#include "commondef.h"
#include "debug.h"

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
{
    instance=this;
}

void DanmakuLyrica::mainInit()
{

    SoundRegistry::createSound("hit0", false, "data/sound/lyrica_notehit0.wav", 1.0f);
    SoundRegistry::createSound("hit1", false, "data/sound/lyrica_notehit1.wav", 1.0f);
    SoundRegistry::createSound("miss", false, "data/sound/miss.wav", 1.0f);

    createTexture("data/tex.png", &tex);
    strip.setTexture(tex);
    strip.setStripPos(0, 0, 0.5, 0.5);
    strip.setMaxIndex(2, 2);

    noteMap.loadTjaFile("data/stage/test/oggtest.lrc");

    char stageDir[200]="data/stage/test/";
    bgm.load(strcat(stageDir, noteMap.getWavFilename()));
    bgm.setVolume(0.5f);

    bgmTimeStamp = 0.0;

    mapState=noteMap.getBgmBeginState();

    script.setTime(mapState.beatOffset);
    script.loadScriptFile("data/stage/test/main.lua");

    noteScene.setNoteMap(&noteMap);

    sceneManager.pushScene(&bulletScene);
    sceneManager.pushScene(&noteScene);

    ImagePiece::loadPending();

}

void DanmakuLyrica::mainCleanup()
{
    sceneManager.clearAllScene();
    bgm.release();
}

void DanmakuLyrica::mainUpdate()
{
    static bool isPaused=true;
    static double timeTotal=0;

    JudgeResult judgeResult;

    // do player motion first
    int flagDir = Dir_None;
    if(buttonUp.isDown())
        flagDir^=Dir_Up;
    if(buttonDown.isDown())
        flagDir^=Dir_Down;
    if(buttonLeft.isDown())
        flagDir^=Dir_Left;
    if(buttonRight.isDown())
        flagDir^=Dir_Right;
    bulletScene.setPlayerMotion((PlayerDirection)flagDir);

    //1: do time line
    //2: update scenes
    //2.1: get process time from lua events
    //2.2: update note scene
    //2.3: update bullet scene and lua events
    //2.4: if still have processing time left, repeat 2

    if(!isPaused)
    {
        double newTime=bgm.getTime()+inputOffset;
        double deltaSec=timeLine.getDeltaTimeFixed(newTime-bgmTimeStamp);
        timeTotal+=deltaSec;

        bgmTimeStamp=newTime;

        while(deltaSec>=M_DINFS)
        {
            BeatTime deltaTime=noteMap.offsetMapStateSingle(mapState, deltaSec);
            deltaSec -= deltaTime.sec;

            double newDelta;

            while(deltaTime.beat>=M_DINFS)
            {
                newDelta=script.seekNextTask(deltaTime.beat);
                BeatTime deltaDelta = deltaTime;
                deltaDelta.offsetBeat(-newDelta);
                sceneManager.updateScene(deltaDelta);
                if(newDelta<=0.0)
                    break;
                deltaTime.resizeByBeat(newDelta);
                script.updateSingleTask(deltaTime.beat);
            }

        }


    }

    if(buttonPause.isPushed())
    {
        isPaused=!isPaused;
        if(isPaused)
        {
            timeLine.pause();
            bgm.pause();
        }
        else
        {
            timeLine.resume();
            bgm.play();
        }
    }

    if(buttonSkip.isPushed())
    {
        double btime=bgm.getTime();
        cout<<btime<<endl;
        bgm.setTime(btime+2);
        //nothing else happens....
    }

    if(buttonA.isPushed() || buttonB.isPushed())
    {
        judgeResult=noteScene.judgeSingleNote(bgmTimeStamp-0.015);
        if(judgeResult==miss || judgeResult== bad)
            SOUND("hit0")->play(true);
        else
            SOUND("hit1")->play(true);
    }

}

void DanmakuLyrica::mainRender()
{
    // render scenes
    d3d.beginScene();

    d3d.pushMatrix();
    sceneManager.renderScene();
    d3d.popMatrix();

    d3d.endScene();

    d3d.present();

}
