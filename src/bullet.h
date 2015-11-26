#ifndef BULLET_H
#define BULLET_H

#include "bulletdef.h"

#include "bulletbase.h"
#include "bullettype.h"
#include "graphics.h"
#include "registry.h"

#define BULLET(x) (Bullet::reg.getId(x))

class Bullet : public BulletBase
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

    Bullet& setBulletType(int type);

protected:

    BulletType* typeInfo;

    bool isOutsideScene(double x1, double y1, double x2, double y2);

    bool onJudge(Entity* entity, double span);

    virtual void onTick();
    virtual void onRender();
    virtual void onDestroy();

    friend class BulletJudge;


};

#endif // BULLET_H
