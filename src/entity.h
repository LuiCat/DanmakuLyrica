#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"

#include "ticking.h"
#include "position.h"

class Entity : public Ticking, public Position
{
public:

    Entity();
    Entity(double posX, double posY);
    Entity(double posX, double posY, double vel, double angle);
    virtual ~Entity();

    virtual void render();

    Entity* offsetMotion(double ticks);

    double getSpeed() const;
    double getRotation() const;
    double getSpeedX();
    double getSpeedY();

    Entity* setSpeed(double value);
    Entity* setRotation(double value);
    Entity* setSpeedRotation(double vel, double angle);

    void correctAxisSpeed();
    void correctVelocityAngle();

    double getFacingAngle() const;
    Entity* setFacingAngle(double value);

    Entity* setForceFacing(bool forced, bool refreshFacing=false);

    Entity* setPositionOffset(double offsetX, double offsetY);
    Entity* setSpeedOffset(double offsetVel);
    Entity* setRotationOffset(double offsetAngle);
    Entity* setVelocityOffset(double offsetVel, double offsetAngle);
    Entity* setAxisSpeedOffset(double offsetVelX, double offsetVelY);

protected:

    double speed, rotation;
    double speedX, speedY;
    double facingAngle;

    bool forceFacing;    

    bool useDefaultMotion;
    bool useDefaultRendering;

    void onUpdateMotion(double deltaSec, double deltaTick);
    void onTick();

    virtual void onRender();
    virtual void destroy();

private:

    bool isAxisVelocityCorrect;

};

#endif // ENTITY_H
