#include "bullet.h"
#include "mathhelper.h"

#include "debug.h"

#include <cstring>

BulletType Bullet::defaultBullet={0, 16, 16, 8, 8, None, Rect, 8, 8};
Registry<BulletType> Bullet::reg;

void Bullet::registerBullet(const char *typeName, Texture texture, double sizeX, double sizeY, double centerX, double centerY, FacingType facingType, JudgeType judgeType, double judgeSizeX, double judgeSizeY)
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
    setForceFacing(typeInfo->facingType==Forward, true);
    return this;
}

void Bullet::onTick()
{

}

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