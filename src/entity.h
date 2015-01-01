#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"

#include "sprite.h"

class Entity : public Sprite
{
public:

    // constructors
    Entity();
    Entity(double posX, double posY);
    Entity(double posX, double posY, double vel, double angle);
    virtual ~Entity();

    // motion properties
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

    // main method making motions
    Entity* offsetMotion(double ticks);

    // inherited render method
    virtual void render();

    // method to make destroying animations & set dead later
    void destroy();

protected:

    double speed, rotation;
    double speedX, speedY;
    double facingAngle;

    bool forceFacing;    

    bool useDefaultMotion;

    virtual void onUpdateMotion(double deltaSec, double deltaTick);
    virtual void onTick();
    virtual void onRender();
    virtual void onDestory();

private:

    bool isAxisVelocityCorrect;

};

#endif // ENTITY_H
