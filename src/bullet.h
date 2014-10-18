class Bullet;

#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

#include "registry.h"

#define BULLET(x) (Bullet::reg.getNameId(x))

enum JudgeType
{
    Round,
    Rect,
    Oval
};

enum FacingType
{
    None,
    Forward,
    Spin
};

struct BulletType
{
    Texture texture;
    double sizeX, sizeY;
    double centerX, centerY;
    FacingType facingType;
    JudgeType judgeType;
    double judgeSizeX, judgeSizeY;
};

class Bullet : public Entity
{
private:

    static BulletType defaultBullet;

public:

    static Registry<BulletType> reg;

    //static void registerBullet(const char *typeName, const BulletType *typeInfo);
    static void registerBullet(const char *typeName, Texture texture,
                               double sizeX, double sizeY, double centerX, double centerY, FacingType facingType,
                               JudgeType judgeType, double judgeSizeX, double judgeSizeY);

    Bullet();
    Bullet(double posX, double posY, double vel, double angle, int type);

    Bullet* setBulletType(int type);

    //friend class LuaScript;

protected:

    //BulletScene *scene;

    BulletType *typeInfo;

    void onTick();
    void onRender();
    void destroy();

};

#endif // BULLET_H
