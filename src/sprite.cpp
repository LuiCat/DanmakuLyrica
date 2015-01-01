#include "sprite.h"

Sprite::Sprite()
    :Ticking()
    ,Position()
{
}

Sprite::Sprite(double posX, double posY)
    :Ticking()
    ,Position(posX, posY)
{

}

void Sprite::render()
{
    if(isDead)return;
    d3d.pushMatrix();
    d3d.translate2D(x, y);
    onRender();
    d3d.popMatrix();
}

void Sprite::onRender()
{
    d3d.pushVertex(0.0, 0.0, 0.0, 0.0);
    d3d.pushVertex(1.0, 0.0, 1.0, 0.0);
    d3d.pushVertex(1.0, 1.0, 1.0, 1.0);
    d3d.pushVertex(0.0, 1.0, 0.0, 1.0);
}
