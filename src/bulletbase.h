#ifndef BULLETBASE_H
#define BULLETBASE_H

#include "entity.h"
#include "bulletdef.h"

class BulletJudge;
class Player;

class BulletBase : public Entity
{
public:

    enum ExtendType
    {
        Type_Unknown = 0,
        Type_Bullet,
        Type_SpiritWheel,
        Type_Enemy,
        Type_Effect
    };

    BulletBase();
    BulletBase(double posX, double posY, double vel, double angle);

    void checkOutsideScene(double x1, double y1, double x2, double y2);

    void setJudge(BulletJudge* judgeObj);

    bool judgePlayer(Player& player);

    virtual ExtendType getType() const = 0;

protected:

    BulletJudge* judge;

    double deadTime;

    virtual bool isOutsideScene(double x1, double y1, double x2, double y2)=0;
    virtual void onOutsideScene();

    virtual bool onJudge(Entity* entity, double span);

    virtual void onTick();
    virtual void onUpdateMotion(double deltaSec, double deltaTick);
    virtual void onDestroy();

};



#endif // BULLETBASE_H
