#include "danmakulyrica.h"

#include "graphics.h"

DanmakuLyrica::DanmakuLyrica()
{
}


void DanmakuLyrica::mainInit()
{
}

void DanmakuLyrica::mainCleanup()
{
}

void DanmakuLyrica::mainUpdate()
{
}

void DanmakuLyrica::mainRender()
{
    static int updateCount=0;

    d3d.beginScene();

    d3d.resetMatrix();
    d3d.pushMatrix();

    d3d.translate(400, 300);
    d3d.scale(150.0, 150.0);
    d3d.rotate(0.085*updateCount++);
    d3d.translate(-0.5, -0.5);

    d3d.pushVertex(0.0, 0.0, 0.0, 0.0);
    d3d.pushVertex(1.0, 0.0, 1.0, 0.0);
    d3d.pushVertex(1.0, 1.0, 1.0, 1.0);
    d3d.pushVertex(0.0, 1.0, 0.0, 1.0);

    d3d.popMatrix();

    d3d.endScene();

    d3d.present();
}
