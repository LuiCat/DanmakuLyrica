#ifndef BULLET_H
#define BULLET_H

#include "bulletdef.h"

#include "entity.h"
#include "registry.h"

#include <list>
using namespace std;

#define BULLET(x) (Bullet::reg.getId(x))

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
    Bullet(double posX, double posY, double vel, double angle, int type, BulletScene* scene=0);

    Bullet& setBulletType(int type);

protected:

    BulletType* typeInfo;

    BulletScene* bulletScene;

    bool checkOutsideScene();

    virtual void onUpdateMotion(double deltaSec, double deltaTick);
    virtual void onTick();
    virtual void onRender();
    virtual void onDestroy();

};

#endif // BULLET_H
