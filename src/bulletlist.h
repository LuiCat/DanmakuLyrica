#ifndef BULLETLIST_H
#define BULLETLIST_H

#include "bulletdef.h"

#include "entitylist.h"
#include "bulletbase.h"

class BulletList : public EntityList<BulletBase>
{
protected:

    BulletJudge* judge;

public:

    BulletList();

    void checkOutsideScene(double x1, double y1, double x2, double y2);
    bool judgeBullets(Player& player);

    template <typename Function>
    void destroyBulletInRange(Function rangeClear)
    {
        for(auto& pair : entityList)
        {
            auto bullet = pair.second;
            if(bullet->destroyed())
                continue;
            if(rangeClear(bullet->getX(), bullet->getY()))
                bullet->destroy();
        }
    }

    void destroyBulletInRect(double x1, double y1, double x2, double y2);
    void destroyBulletInCircle(double x, double y, double r);

    void destroyAll();

};


#endif // BULLETLIST_H
