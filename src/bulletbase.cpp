#include "bulletbase.h"
#include "bulletjudge.h"

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

bool BulletBase::onJudge(Entity*, double)
{
    return false;
}

void BulletBase::onUpdateMotion(double deltaSec, double deltaTick)
{
    if(judge)
    {
        judge->updateModelBefore(this);
        Entity::onUpdateMotion(deltaSec, deltaTick);
        judge->updateModelAfter(this);
    }
    else
    {
        Entity::onUpdateMotion(deltaSec, deltaTick);
    }
}

void BulletBase::checkOutsideScene(double x1, double y1, double x2, double y2)
{
    if(isOutsideScene(x1, y1, x2, y2))
    {
        onOutsideScene();
    }
}

void BulletBase::setJudge(BulletJudge* judgeObj)
{
    judge = judgeObj;
}

bool BulletBase::judgePlayer(Player& player)
{
    return judge!=0 && onJudge(&player, player.getJudgeSpan());
}


