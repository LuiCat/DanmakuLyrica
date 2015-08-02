#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "entityevent.h"

class Movement : public EntityEvent
{
public:

    enum MoveType
    {
        none, position, posx, posy, speedangle, speed, angle,
        axisspeed, xspeed, yspeed, acceleration, rotatespeed, facing
    };

    Movement(double time, MoveType type=none, bool relative=false, double param1=0.0, double param2=0.0);

protected:

    void onProcess(Entity* entity);

private:

    MoveType moveType;
    bool relative;
    double param1;
    double param2;

};

#endif // MOVEMENT_H
