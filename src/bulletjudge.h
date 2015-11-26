#ifndef BULLETJUDGE_H
#define BULLETJUDGE_H

#include "bullet.h"
#include "player.h"

class BulletJudge
{
protected:

    BulletList* list;

public:

    BulletJudge();

    // Update model on position of bullet, benefit to fast judge
    void updateModelBefore(BulletBase* bullet);
    void updateModelAfter(BulletBase* bullet);

    // Returns true if collision occur. Toggle fast judge by passing true to fullJudge
    bool judgeBullet(Bullet* bullet, Entity* judgeObj, double span, bool fastJudge=false);

    // bool judgeOther(Other& bullet, Entity& judgeObj, double span, bool fastJudge=false);


};

#endif // BULLETJUDGE_H
