#include "bulletlist.h"
#include "player.h"
#include "debug.h"

BulletList::BulletList()
    :EntityList()
{

}

void BulletList::checkOutsideScene(double x1, double y1, double x2, double y2)
{
    forEach([=](BulletBase& b){b.checkOutsideScene(x1, y1, x2, y2);});
}

bool BulletList::judgeBullets(Player& player)
{
    bool res = false;
    for(auto& pair : entityList)
    {
        auto bullet = pair.second.get();
        if(bullet->destroyed())
            continue;
        res |= bullet->judgePlayer(player);
        if(res)break;
    }
    return res;
}

int BulletList::destroyBulletInRect(double x1, double y1, double x2, double y2)
{
    int res=0;
    for(auto& pair : entityList)
    {
        auto bullet = pair.second.get();
        if(bullet->destroyed() || bullet->getType() != BulletBase::Type_Bullet)
            continue;
        double x = bullet->getX();
        double y = bullet->getY();
        if(x>x1 && x<x2 && y>y1 && y<y2)
        {
            bullet->destroy();
            ++res;
        }
    }
    return res;
}

int BulletList::destroyBulletInCircle(double x, double y, double r)
{
    int res=0;
    for(auto& pair : entityList)
    {
        auto bullet = pair.second.get();
        if(bullet->destroyed() || bullet->getType() != BulletBase::Type_Bullet)
            continue;
        if(dist(bullet->getX()-x, bullet->getY()-y)<r)
        {
            bullet->destroy();
            ++res;
        }
    }
    return res;
}

void BulletList::destroyAllBullets()
{
    for(auto& pair : entityList)
    {
        auto bullet = pair.second.get();
        if(bullet->getType() != BulletBase::Type_Bullet)
            continue;
        bullet->destroy();
    }
}

void BulletList::destroyAll()
{
    for(auto& pair : entityList)
    {
        auto bullet = pair.second.get();
        bullet->destroy();
    }
}

SpiritWheel* BulletList::getSpirit(int id)
{
    auto iter = entityList.find(id);
    if(iter == entityList.end() || iter->second->getType() != BulletBase::Type_SpiritWheel)
        return nullptr;
    return dynamic_cast<SpiritWheel*>(iter->second.get());
}
