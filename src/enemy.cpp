#include "enemy.h"


Enemy::Enemy()
    :BulletBase(0, -150, 0, 0)
{
    setForceFacing(false);
    img = TexturePiece::createImageSet("data/image/enemy/flan.png", 2, 3);
    wheel.load("data/image/enemy/wheel.png");
}

void Enemy::onTick()
{

}

void Enemy::onRender()
{
    d2d.pushMatrix();
    d2d.rotate(-timeSec*M_PI*0.2);
    d2d.setColor(0xF34343);
    wheel.vertice(0.5, 0.5, 280);
    d2d.popMatrix();

    d2d.pushMatrix();
    d2d.translate(0, 10*sin(timeSec*M_PI*0.25));
    d2d.setAlpha(0.5);
    img[3+((int)round(timeSec))%3].vertice(0.5, 0.5, 150);
    d2d.setAlpha(1.0);
    img[((int)round(timeSec))%3].vertice(0.5, 0.5, 128);
    d2d.popMatrix();
}

void Enemy::onDestroy()
{
    setDead();
}

bool Enemy::isOutsideScene(double x1, double y1, double x2, double y2)
{
    return false;
}
