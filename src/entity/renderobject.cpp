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
    d2d.pushMatrix();
    d2d.translate(x, y);
    onRender();
    d2d.popMatrix();
}

void RenderObject::onRender()
{
    d2d.rect(0, 0, 1, 1);
}
