#include "renderobject.h"

RenderObject::RenderObject()
    :Ticking()
    ,Position()
{
}

RenderObject::RenderObject(double posX, double posY)
    :Ticking()
    ,Position(posX, posY)
{

}

void RenderObject::render()
{
    if(isDead)return;
    d3d.pushMatrix();
    d3d.translate2D(x, y);
    onRender();
    d3d.popMatrix();
}

void RenderObject::onRender()
{
    d3d.vertex(0.0, 0.0, 0.0, 0.0);
    d3d.vertex(1.0, 0.0, 1.0, 0.0);
    d3d.vertex(1.0, 1.0, 1.0, 1.0);
    d3d.vertex(0.0, 1.0, 0.0, 1.0);
}
