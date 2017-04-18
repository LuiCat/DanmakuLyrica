#include "enemy.h"


Enemy::Enemy()
    :BulletBase(0, -150, 0, 0)
{
    setForceFacing(false);
    img = TexturePiece::createImageSet("data/image/enemy/flan.png", 1, 4, 3);
    wheel.load("data/image/enemy/wheel.png");
}

void Enemy::onTick()
{

}

void Enemy::onRender()
{
    d3d.pushMatrix();
    d3d.rotate2D(-timeSec*M_PI*0.2);
    d3d.setColor(0xBB5555);
    wheel.vertice(0.5, 0.5, 280);
    d3d.popMatrix();

    d3d.pushMatrix();
    d3d.translate2D(0, 10*sin(timeSec*M_PI*0.25));
    img[((int)round(timeSec))%3].vertice(0.5, 0.5, 128);
    d3d.popMatrix();
}

void Enemy::onDestroy()
{
    setDead();
}

bool Enemy::isOutsideScene(double x1, double y1, double x2, double y2)
{
    return false;
}
