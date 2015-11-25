#include "movement.h"
#include "entity.h"

Movement::Movement(double time, Movement::MoveType type, bool relative, double param1, double param2)
    :EntityEvent(time)
    ,moveType(type)
    ,relative(relative)
    ,param1(param1)
    ,param2(param2)
{

}

void Movement::onProcess(Entity* entity)
{
    switch (moveType) {
    case position:
        if(relative)
            entity->setPositionOffset(param1, param2);
        else
            entity->setPosition(param1, param2);
        break;
    case posx:
        entity->setX(relative?entity->getX()+param1:param1);
        break;
    case posy:
        entity->setY(relative?entity->getY()+param1:param1);
        break;
    case speedangle:
        relative?entity->setVelocityOffset(param1, param2):entity->setSpeedRotation(param1, param2);
        break;
    case speed:
        relative?entity->setSpeedOffset(param1):entity->setSpeed(param1);
        break;
    case angle:
        relative?entity->setRotationOffset(param1):entity->setRotation(param1);
        break;
    case axisspeed:
        relative?entity->setAxisSpeedOffset(param1, param2):entity->setAxisSpeed(param1, param2);
        break;
    case xspeed:
        entity->setSpeedX(relative?entity->getSpeedX()+param1:param1);
        break;
    case yspeed:
        entity->setSpeedY(relative?entity->getSpeedY()+param1:param1);
        break;
    case acceleration:
        entity->setAccelerateSpeed(relative?entity->getAccelerateSpeed()+param1:param1);
        break;
    case rotatespeed:
        entity->setRotateSpeed(relative?entity->getRotateSpeed()+param1:param1);
        break;
    case facing:
        entity->setFacingAngle(relative?entity->getFacingAngle()+param1:param1);
        break;
    default:
        break;
    }
}
