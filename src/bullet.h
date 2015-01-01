class Bullet;

#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

#include "registry.h"

#include <list>
using namespace std;

#define BULLET(x) (Bullet::reg.getNameId(x))

struct BulletType
{
    Texture texture;
    double sizeX, sizeY;
    double centerX, centerY;

    enum FacingType
    {
        none, forward, spin
    }facingType;

    enum JudgeType
    {
        round, rect, oval
    }judgeType;

    double judgeSizeX, judgeSizeY;
};

enum AnimType
{
    x,y,speed,angle,vx,vy
};

class Bullet : public Entity
{
private:

    // default bullet for registry quest
    static BulletType defaultBullet;

public:

    // bullet registry
    static Registry<BulletType> reg;

    // bullet registry funcs
    static void registerBullet(const char *typeName, const BulletType *typeInfo);
    static void registerBullet(const char *typeName, Texture texture,
                               double sizeX, double sizeY, double centerX, double centerY, BulletType::FacingType facingType,
                               BulletType::JudgeType judgeType, double judgeSizeX, double judgeSizeY);

    // constructors
    Bullet();
    Bullet(double posX, double posY, double vel, double angle, int type);

    Bullet* setBulletType(int type);

    // animation funcs
    Bullet* animate(AnimType type, double deltaValue, double deltaSec);
    Bullet* animateTo(AnimType type, double value, double deltaSec);
    Bullet* clearAnim();
    Bullet* finishAnim();

protected:

    BulletType *typeInfo;

    void onTick();
    void onRender();
    void destroy();

};

#endif // BULLET_H
