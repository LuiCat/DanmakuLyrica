#include "bulletlist.h"
#include "player.h"

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
        res |= pair.second->judgePlayer(player);
        if(res)
            break;
    }
    return res;
}

void BulletList::destroyBulletInRect(double x1, double y1, double x2, double y2)
{
    for(auto& pair : entityList)
    {
        auto bullet = pair.second;
        if(bullet->destroyed())
            continue;
        double x = bullet->getX();
        double y = bullet->getY();
        if(x>x1 && x<x2 && y>y1 && y<y2)
            bullet->destroy();
    }
}

void BulletList::destroyBulletInCircle(double x, double y, double r)
{
    for(auto& pair : entityList)
    {
        auto bullet = pair.second;
        if(bullet->destroyed())
            continue;
        if(dist(bullet->getX(), bullet->getY())<r)
            bullet->destroy();
    }
}

void BulletList::destroyAll()
{
    for(auto& pair : entityList)
    {
        auto bullet = pair.second;
        bullet->destroy();
    }
}
