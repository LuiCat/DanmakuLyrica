#include "entityevent.h"
#include "entity.h"

EntityEvent::EntityEvent(double processTime)
    :processTime(processTime)
    ,processed(false)
{

}

EntityEvent::~EntityEvent()
{

}

double EntityEvent::process(Entity* entity, double deltaTime)
{
    if(entity==0||processed)
        return deltaTime;

    double curTime=entity->getTimeSec();

    if(processTime<curTime)
    {
        processed=true;
        return deltaTime;
    }

    if(curTime+deltaTime<processTime)
    {
        entity->offsetMotion(deltaTime);
        return 0.0;
    }

    double offsetTime=processTime-curTime;

    entity->offsetMotion(offsetTime);
    onProcess(entity);
    processed=true;

    return deltaTime-offsetTime;
}

