#include "danmakulyrica.h"

#include "graphics.h"

#include "commondef.h"

#include "soundregistry.h"

DanmakuLyrica::DanmakuLyrica()
    :buttonA(DIK_Z, 0)
    ,buttonB(DIK_X, 0)
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

}

void DanmakuLyrica::mainCleanup()
{
    bgm.release();
    noteScene.cleanup();
}

void DanmakuLyrica::mainUpdate()
{
    static double timeStamp=-1;
    if(timeStamp<0)
    {
        timeStamp=0;
        timeLine.reset();
        bgm.play(true);
    }
    double newTime=bgm.getTime();
    noteScene.update(timeLine.getDeltaTimeFixed(newTime-timeStamp));
    timeStamp=newTime;

    if(buttonA.isPushed())
        SOUND("hit1")->play(true);
    if(buttonB.isPushed())
        SOUND("hit1")->play(true);

}

void DanmakuLyrica::mainRender()
{
    d3d.beginScene();

    d3d.resetMatrix();

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
    d3d.endScene();

    d3d.present();
}
