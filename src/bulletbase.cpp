#include "bulletbase.h"
#include "bulletjudge.h"
#include "debug.h"

BulletBase::BulletBase()
    :Entity()
    ,judge(nullptr)
{
    setTickRate(10);
}

BulletBase::BulletBase(double posX, double posY, double vel, double angle)
    :Entity(posX, posY, vel, angle)
    ,judge(nullptr)
{
    setTickRate(10);
}

void BulletBase::onOutsideScene()
{
    setDead();
}

bool BulletBase::onJudge(Entity*, double)
{
    return false;
}

void BulletBase::onTick()
{
    if(isDestroyed && timeSec>deadTime)
        setDead();
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

void BulletBase::onDestroy()
{
    deadTime = getTimeSec() + 1.0;
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
    return (judge!=nullptr) && (!isDestroyed)
            && onJudge(&player, player.getJudgeSpan());
}


