#ifndef BULLETLIST_H
#define BULLETLIST_H

#include "entitylist.h"

#include "bullet.h"

class BulletList : public EntityList<Bullet>
{
public:
    BulletList();
};

#endif // BULLETLIST_H
