#ifndef BULLETLIST_H
#define BULLETLIST_H

#include "bulletdef.h"

#include "entitylist.h"
#include "bulletbase.h"
#include "spiritwheel.h"

class BulletList : public EntityList<BulletBase>
{
protected:

    //BulletJudge* judge;

public:

    BulletList();

    void checkOutsideScene(double x1, double y1, double x2, double y2);
    bool judgeBullets(Player& player);

    template <typename Function>
    int destroyInRange(Function rangeClear)
    {
        int res=0;
        for(auto& pair : entityList)
        {
            auto bullet = pair.second;
            if(bullet->destroyed())
                continue;
            if(rangeClear(bullet->getX(), bullet->getY()))
            {
                bullet->destroy();
                ++res;
            }
        }
        return res;
    }

    int destroyBulletInRect(double x1, double y1, double x2, double y2);
    int destroyBulletInCircle(double x, double y, double r);

    void destroyAllBullets();
    void destroyAll();

    SpiritWheel* getSpirit(int id);

};


#endif // BULLETLIST_H
