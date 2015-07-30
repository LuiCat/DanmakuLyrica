#include "danmakulyrica.h"

#include "graphics.h"
#include "soundregistry.h"

#include "mathhelper.h"
#include "commondef.h"
#include "debug.h"

DanmakuLyrica* DanmakuLyrica::instance = 0;

DanmakuLyrica::DanmakuLyrica()
    :script(&bulletScene)
    ,bgmTimeStamp(0.0)
    ,buttonA(DIK_Z, 0)
    ,buttonB(DIK_X, 0)
    ,buttonPause(DIK_SPACE, 0)
    ,buttonSkip(DIK_S, 0)
{
    instance=this;
}

void DanmakuLyrica::mainInit()
{

    SoundRegistry::newSound("hit0", false, "data/sound/lyrica_notehit0.wav", 1.0f);
    SoundRegistry::newSound("hit1", false, "data/sound/lyrica_notehit1.wav", 1.0f);

    createTexture("data/tex.png", &tex);
    strip.setTexture(tex);
    strip.setStripPos(0, 0, 0.5, 0.5);
    strip.setMaxIndex(2, 2);

    noteMap.loadTjaFile("data/stage/test/oggtest.lrc");

    char stageDir[200]="data/stage/test/";
    bgm.loadWav(strcat(stageDir, noteMap.getWavFilename()));
    bgm.setVolume(0.5f);

    mapState=noteMap.getBgmBeginState();

    script.setTime(mapState.beatOffset);
    script.loadScriptFile("data/stage/test/main.lua");

    noteScene.setNoteMap(&noteMap);

    sceneManager.pushScene(&bulletScene);
    sceneManager.pushScene(&noteScene);

}

void DanmakuLyrica::mainCleanup()
{
    sceneManager.clearAllScene();
    bgm.release();
}

void DanmakuLyrica::mainUpdate()
{
    static bool isPaused=true;
    static double timeTwigger=0;

    JudgeResult judgeResult;

    //1: do time line
    //2: update scenes
    //2.1: get process time from lua events
    //2.2: update note scene
    //2.3: update bullet scene and lua events
    //2.4: if still have processing time left, repeat 2

    if(!isPaused)
    {
        double newTime=bgm.getTime()+timeTwigger;
        double deltaTime=timeLine.getDeltaTimeFixed(newTime-bgmTimeStamp);
        double deltaBeat=noteMap.offsetMapState(mapState, deltaTime);
        double newDelta;

        bgmTimeStamp=newTime;

        while(deltaBeat>0)
        {
            newDelta=script.seekNextTask(deltaBeat);
            deltaBeat-=newDelta;
            sceneManager.updateScene(deltaBeat);
            deltaBeat=newDelta;
            if(deltaBeat<=0.0)
                break;
            script.updateSingleTask(deltaBeat);
        }

    }

    if(buttonPause.isPushed())
    {
        isPaused=!isPaused;
        if(isPaused)
        {
            bgm.pause();
        }
        else
        {
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
