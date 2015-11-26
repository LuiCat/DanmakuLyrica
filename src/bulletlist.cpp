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

