#include "bullet.h"
#include "bulletscene.h"
#include "movement.h"
#include "bulletjudge.h"

#include "mathhelper.h"
#include "debug.h"

#include <cstring>

BulletType Bullet::defaultBullet={ImagePiece(), 16, 16, 8, 8, BulletType::none, BulletType::rect, 8, 8};
Registry<BulletType> Bullet::reg;

void Bullet::registerBullet(const char *typeName, const BulletType& typeInfo)
{
    reg.registerName(typeName, typeInfo);
}

void Bullet::registerBullet(const char *typeName, Texture texture, double sizeX, double sizeY, double centerX, double centerY, BulletType::FacingType facingType, BulletType::JudgeType judgeType, double judgeSizeX, double judgeSizeY)
{
    BulletType info={texture, sizeX, sizeY, centerX, centerY, facingType, judgeType, judgeSizeX, judgeSizeY};
    reg.registerName(typeName, std::move(info));
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
    if(typeInfo == nullptr)
        return;
    d3d.pushMatrix();
    typeInfo->image.vertice(typeInfo->centerX, typeInfo->centerY, typeInfo->sizeX, typeInfo->sizeY);
    d3d.popMatrix();
}

void Bullet::onDestroy()
{
    setDead();
}

bool Bullet::onJudge(Entity* entity, double span)
{
    return judge->judgeBullet(this, entity, span);
}
