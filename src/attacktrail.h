#ifndef ATTACKTRAIL_H
#define ATTACKTRAIL_H

#include "bulletbase.h"

#include <vector>
using namespace std;

class AttackTrail : public BulletBase
{
public:

    AttackTrail(int x, int y, int targetX, int targetY);

    ExtendType getType() const
    {
        return Type_Effect;
    }

protected:

    double w;
    double len;

    void onTick();
    void onRender();

    bool isOutsideScene(double x1, double y1, double x2, double y2);

};

#endif // ATTACKTRAIL_H
