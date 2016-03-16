#include "bulletjudge.h"
#include "commondef.h"
#include "debug.h"

BulletJudge::BulletJudge()
    : list(nullptr)
{

}

void BulletJudge::updateModelBefore(BulletBase* bullet)
{
    UNUSED(bullet);
    // undealt
}

void BulletJudge::updateModelAfter(BulletBase* bullet)
{
    UNUSED(bullet);
    // undealt
}

bool BulletJudge::judgeBullet(Bullet* bullet, Entity* judgeObj, double span, bool fastJudge /* undealt */)
{
    UNUSED(fastJudge);

    if(!bullet || !judgeObj)
    {
        return false;
    }

    auto type = bullet->typeInfo;
    if(!type)
    {
        return false;
    }

    // Add span to sizeX
    double sx = type->judgeSizeX+span;

    // Round judge treat sizeX as radius
    if(type->judgeType == BulletType::round)
    {
        if(judgeObj->distanceTo(*bullet)<sx)
            return true;
        return false;
    }

    // Do judges depend on bullet rotation

    // Add span to sizeY
    double sy = type->judgeSizeY+span;

    // Get relative position on original coordinate
    double ox = judgeObj->getX()-bullet->getX();
    double oy = judgeObj->getY()-bullet->getY();

    // Position on rotated coordinate, according to bullet's facing angle
    double angle = bullet->getFacingAngle();
    double nx = ox*cos(angle)-oy*sin(angle);
    double ny = ox*sin(angle)+oy*cos(angle);

    switch(type->judgeType)
    {
    case BulletType::rect:
        return abs(nx)*2<sx && abs(ny)*2<sy;
    case BulletType::oval:
        return dist(nx/sx, ny/sy)<0.5;
    default:
        return false;
    }

    // Oops.
    return false;

}

