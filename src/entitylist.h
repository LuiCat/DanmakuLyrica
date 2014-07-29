#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include "entity.h"

#include <list>
using namespace std;

class EntityList
{
public:

    EntityList();

    void updateAll(double deltaSec);
    void renderAll();

    void clearAll();
    void clearDead();

    void setDeadAutoClear(bool clear);

protected:

    list<Entity> entityList;

private:

    bool autoClear;

};

#endif // ENTITYLIST_H
