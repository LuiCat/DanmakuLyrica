#include "bulletbase.h"

BulletBase::BulletBase()
    :Entity()
{

}

BulletBase::BulletBase(double posX, double posY, double vel, double angle)
    :Entity(posX, posY, vel, angle)
{

}

void BulletBase::onOutsideScene()
{
    setDead();
}

void BulletBase::checkOutsideScene(double x1, double y1, double x2, double y2)
{
    if(isOutsideScene(x1, y1, x2, y2))
    {
        onOutsideScene();
    }
}


