#ifndef ENTITYEVENT_H
#define ENTITYEVENT_H

#include "entity.h"
#include "mathhelper.h"

class Entity;

class EntityEvent
{
protected:

    double processTime;
    bool processed;

    virtual void onProcess(Entity* entity)=0;

public:

    EntityEvent(double processTime);

    double process(Entity* entity, double deltaTime=M_DINF);

    inline bool isProcessed() const
    {
        return processed;
    }

};

#endif // ENTITYEVENT_H
