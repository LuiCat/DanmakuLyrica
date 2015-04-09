#ifndef BULLETBASE_H
#define BULLETBASE_H

#include "entity.h"
#include "bulletdef.h"

class BulletBase : public Entity
{
public:

    BulletBase();
    BulletBase(double posX, double posY, double vel, double angle);

    void checkOutsideScene(double x1, double y1, double x2, double y2);

protected:

    virtual bool isOutsideScene(double x1, double y1, double x2, double y2)=0;
    virtual void onOutsideScene();

};



#endif // BULLETBASE_H
