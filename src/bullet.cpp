#include "bullet.h"
#include "bulletscene.h"

#include "mathhelper.h"
#include "debug.h"

#include <cstring>

BulletType Bullet::defaultBullet={0, 16, 16, 8, 8, BulletType::none, BulletType::rect, 8, 8};
Registry<BulletType> Bullet::reg;

void Bullet::registerBullet(const char *typeName, const BulletType *typeInfo)
{
    reg(typeName, typeInfo);
}

void Bullet::registerBullet(const char *typeName, Texture texture, double sizeX, double sizeY, double centerX, double centerY, BulletType::FacingType facingType, BulletType::JudgeType judgeType, double judgeSizeX, double judgeSizeY)
{
    BulletType info={texture, sizeX, sizeY, centerX, centerY, facingType, judgeType, judgeSizeX, judgeSizeY};
    reg(typeName, &info);
}

//===================================================

Bullet::Bullet()
    :BulletBase()
{
    setBulletType(0);
}

Bullet::Bullet(double posX, double posY, double vel, double angle, int type)
    :BulletBase(posX, posY, vel, angle)
{
    setBulletType(type);
}

Bullet& Bullet::setBulletType(int type)
{
    typeInfo=reg.getInfo(type);
    if(typeInfo==0)typeInfo=&defaultBullet;
    setForceFacing(typeInfo->facingType==BulletType::forward, true);
    return *this;
}

bool Bullet::isOutsideScene(double x1, double y1, double x2, double y2)
{
    double tx=getX();
    double ty=getY();
    double sx=getSpeedX();
    double sy=getSpeedY();
    return (tx<x1&&sx<0)||(tx>x2&&sx>0)||(ty<y1&&sy<0)||(ty>y2&&sy>0);
}

void Bullet::onTick(){}

void Bullet::onRender()
{
    d3d.pushMatrix();

    d3d.translate2D(-typeInfo->centerX, -typeInfo->centerY);
    d3d.scale2D(typeInfo->sizeX, typeInfo->sizeY);
    d3d.setTexture(typeInfo->texture);

    d3d.vertex(0.0, 0.0, 0.0, 0.0);
    d3d.vertex(1.0, 0.0, 1.0, 0.0);
    d3d.vertex(1.0, 1.0, 1.0, 1.0);
    d3d.vertex(0.0, 1.0, 0.0, 1.0);

    d3d.popMatrix();
}

void Bullet::onDestroy()
{
    setDead();
}
