#include "danmakulyrica.h"

#include "graphics.h"

#include "commondef.h"

#include "soundregistry.h"

#include "debug.h"

DanmakuLyrica::DanmakuLyrica()
    :buttonA(DIK_Z, 0)
    ,buttonB(DIK_X, 0)
    ,buttonPause(DIK_SPACE, 0)
    ,buttonSkip(DIK_S, 0)
{
}

void DanmakuLyrica::mainInit()
{
    char filename[200]="data/";

    noteScene.init();

    bgm.loadWav(strcat(filename, noteScene.getBgmFilename()));
    bgm.setVolume(0.5f);

    SoundRegistry::registerSoundFile("hit0", false, "data/lyrica_notehit0.wav", 1.0f);
    SoundRegistry::registerSoundFile("hit1", false, "data/lyrica_notehit1.wav", 1.0f);

    createTexture("data/tex.png", &tex);
    strip.setTexture(tex);
    strip.setStripPos(0, 0, 0.5, 0.5);
    strip.setMaxIndex(2, 2);

    LuaScript::init();
    LuaTaskList::registerLuaFuncs(_L);

    LuaScript::loadScriptFile("data/main.lua");

    Texture tex;
    createTexture("data/etama2.png", &tex);
    Bullet::registerBullet("rice", tex, 16, 16, 8, 8, BulletType::forward, BulletType::round, 4, 4);

    LuaTaskList::bulletScene=&bulletScene;

}

void DanmakuLyrica::mainCleanup()
{
    LuaScript::cleanup();
    bgm.release();
    noteScene.cleanup();
}

void DanmakuLyrica::mainUpdate()
{
    static double timeStamp=0;
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
        double deltaTime=timeLine.getDeltaTimeFixed(newTime-timeStamp);
        double newDelta;
        while(deltaTime>0)
        {
            newDelta=taskList.singleTick(deltaTime);
            deltaTime-=newDelta;
            noteScene.update(deltaTime);
            bulletScene.update(deltaTime);
            deltaTime=newDelta;
        }

        timeStamp=newTime;
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
        bgm.setTime(btime+10);
        //nothing else happens....
    }

    if(buttonA.isPushed() || buttonB.isPushed())
    {
        judgeResult=noteScene.judgeSingleNote(timeStamp-0.015);
        if(judgeResult==miss || judgeResult== bad)
            SOUND("hit0")->play(true);
        else
            SOUND("hit1")->play(true);
    }

}

void DanmakuLyrica::mainRender()
{
    d3d.beginScene();

    d3d.resetMatrix();

    /*
    d3d.pushMatrix();
    d3d.scale2D(100.0, 100.0);
    strip.pushVertices(0, 0);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(0, 1);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(1, 0);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(1, 1);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(0, 0);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(1, 0);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(0, 1);
    d3d.translate2D(1.0, 0.0);
    strip.pushVertices(1, 1);
    d3d.popMatrix();
    */

    d3d.pushMatrix();
    d3d.translate2D(100, 300.0);    

    d3d.pushMatrix();
    d3d.setColor(0x00FF00);
    d3d.pushVertex( 0.5,  20, 0.0, 0.0);
    d3d.pushVertex(-0.5,  20, 1.0, 0.0);
    d3d.pushVertex(-0.5, -20, 1.0, 1.0);
    d3d.pushVertex( 0.5, -20, 0.0, 1.0);
    d3d.popMatrix();

    d3d.pushMatrix();
    noteScene.render();
    d3d.popMatrix();

    d3d.popMatrix();

    d3d.pushMatrix();
    bulletScene.render();
    d3d.popMatrix();

    d3d.endScene();

    d3d.present();
}
