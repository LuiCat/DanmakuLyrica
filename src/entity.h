#ifndef ENTITY_H
#define ENTITY_H

#include "graphics.h"

#include "sprite.h"
#include "entityevent.h"

#include <queue>
using namespace std;

class EntityEvent;

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

    Entity& setSpeed(double value);
    Entity& setRotation(double value);
    Entity& setSpeedRotation(double vel, double angle);
    Entity& setSpeedX(double velX);
    Entity& setSpeedY(double velY);
    Entity& setAxisSpeed(double velX, double velY);

    void correctAxisSpeed();
    void correctVelocityAngle();

    double getFacingAngle() const;
    Entity& setFacingAngle(double value);

    Entity& setForceFacing(bool forced, bool refreshFacing=false);

    Entity& setPositionOffset(double offsetX, double offsetY);
    Entity& setSpeedOffset(double offsetVel);
    Entity& setRotationOffset(double offsetAngle);
    Entity& setVelocityOffset(double offsetVel, double offsetAngle);
    Entity& setAxisSpeedOffset(double offsetVelX, double offsetVelY);

    double getAccelerateSpeed() const;
    double getRotateSpeed() const;
    Entity& setAccelerateSpeed(double accVel);
    Entity& setRotateSpeed(double rotateVel);

    template <typename Event>
    inline void pushEvent(const Event& event)
    {
        static_assert(std::is_base_of<EntityEvent, Event>::value,
                      "Parameter of pushEvent(const Event&) is not of subclass of EntityEvent");
        eventList.push(new Event(event));
    }

    // main method making motions
    Entity& offsetMotion(double secs);

    // inherited render method
    virtual void render();

    // method to make destroying animations & set dead later
    void destroy();

protected:

    double speed, rotation;
    double speedX, speedY;
    double accelerateSpeed, rotateSpeed;
    double facingAngle;

    bool forceFacing;    

    bool useDefaultMotion;

    priority_queue<EntityEvent*> eventList;

    virtual void onUpdateMotion(double deltaSec, double deltaTick);
    virtual void onTick();
    virtual void onRender();
    virtual void onDestory();

private:

    bool isAxisVelocityCorrect;

};

#endif // ENTITY_H
