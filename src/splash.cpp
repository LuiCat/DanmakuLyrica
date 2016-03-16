#include "splash.h"

ImagePiece Splash::img("data/image/effect/blow.png");


Splash::Splash(double x, double y)
    :BulletBase(x, y, 0, 0)
{
    setTickRate(1.1);
}

bool Splash::isOutsideScene(double x1, double y1, double x2, double y2)
{
    return false;
}

void Splash::onTick()
{
    if(timeSec>1)
        setDead();
}

void Splash::onRender()
{
    d3d.setAlpha(1-timeSec);
    img.vertice(0.5, 0.5, timeSec*400);
}
