#include "spiritwheel.h"

#include "commondef.h"

ImagePiece SpiritWheel::imgWheel("data/image/enemy/wheel.png");

SpiritWheel::SpiritWheel(double posX, double posY, double vel, double angle, double life, int hp)
    : BulletBase(posX, posY, vel, angle)
    , life(life)
    , maxHp(hp)
    , currHit(0)
{
    setTickRate(30);
    setForceFacing(false);
}

bool SpiritWheel::hit(int times)
{
    if(isDestroyed)
        return false;
    currHit+=times;
    if(currHit<maxHp)
        return false;
    currHit=maxHp;
    destroy();
    return true;
}

void SpiritWheel::onTick()
{
    if(timeSec>life)
        destroy();
    if(isDestroyed && !isDead && timeSec-destroyTime>1.0)
    {
        setDead();
    }
}

void SpiritWheel::onRender()
{
    d3d.rotate2D(timeSec*M_PI_2);
    d3d.setAlpha(0.6);
    if(isDestroyed)
    {
        double scale = timeSec-destroyTime+1.0;
        d3d.scale2D(scale, scale);
        d3d.setAlpha(0.6-(timeSec-destroyTime)*0.6);
    }
    else if(timeSec<0.5)
    {
        d3d.setAlpha(timeSec*2);
    }
    d3d.setColor(MAKE_RGB(255, 255-currHit*50/maxHp, 240-currHit*200/maxHp));
    imgWheel.vertice(0.5, 0.5, 120.0);
}

void SpiritWheel::onDestroy()
{

}

bool SpiritWheel::isOutsideScene(double x1, double y1, double x2, double y2)
{
    return false;
}
