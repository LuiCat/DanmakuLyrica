#include "bullet.h"
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
    :Entity()
{
    setBulletType(0);
}

Bullet::Bullet(double posX, double posY, double vel, double angle, int type)
    :Entity(posX, posY, vel, angle)
{
    setBulletType(type);
}

Bullet* Bullet::setBulletType(int type)
{
    typeInfo=reg.getIdInfo(type);
    if(typeInfo==0)typeInfo=&defaultBullet;
    setForceFacing(typeInfo->facingType==BulletType::forward, true);
    return this;
}

Bullet *Bullet::animate(AnimType type, double deltaValue, double deltaSec)
{
    return this;
}

Bullet *Bullet::animateTo(AnimType type, double value, double deltaSec)
{
    return this;
}

Bullet *Bullet::clearAnim()
{
    return this;
}

Bullet *Bullet::finishAnim()
{
    return this;
}

void Bullet::onTick(){}

void Bullet::onRender()
{
    d3d.pushMatrix();

    d3d.translate2D(-typeInfo->centerX, -typeInfo->centerY);
    d3d.scale2D(typeInfo->sizeX, typeInfo->sizeY);
    d3d.setTexture(typeInfo->texture);

    d3d.pushVertex(0.0, 0.0, 0.0, 0.0);
    d3d.pushVertex(1.0, 0.0, 1.0, 0.0);
    d3d.pushVertex(1.0, 1.0, 1.0, 1.0);
    d3d.pushVertex(0.0, 1.0, 0.0, 1.0);

    d3d.popMatrix();
}

void Bullet::destroy()
{
    setDead();
}
