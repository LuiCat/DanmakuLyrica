#ifndef BULLETLIST_H
#define BULLETLIST_H

#include "bulletdef.h"

#include "entitylist.h"
#include "bulletbase.h"

class BulletList : public EntityList<BulletBase>
{
public:

    BulletList();

    void checkOutsideScene(double x1, double y1, double x2, double y2);

};

#endif // BULLETLIST_H
