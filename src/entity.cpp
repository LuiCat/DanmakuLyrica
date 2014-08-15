#include "entity.h"

#include "mathhelper.h"

//===================================================

double Entity::getSpeed() const
{
    return speed;
}

double Entity::getRotation() const
{
    return rotation;
}

double Entity::getSpeedX()
{
    correctAxisSpeed();
    return speedX;
}

double Entity::getSpeedY()
{
    correctAxisSpeed();
    return speedY;
}

double Entity::getFacingAngle() const
{
    return facingAngle;
}

Entity* Entity::setSpeed(double value)
{
    speed=value;
    if(speed<0.0)
    {
        speed=-speed;
        rotation+=M_PI;
    }
    isAxisVelocityCorrect=false;
    return this;
}

Entity* Entity::setRotation(double value)
{
    rotation=value;
    isAxisVelocityCorrect=false;
    if(forceFacing)
        setFacingAngle(rotation);
    return this;
}

Entity *Entity::setSpeedRotation(double vel, double angle)
{
    setSpeed(vel);
    setRotation(angle);
    correctAxisSpeed();
    return this;
}

Entity* Entity::setFacingAngle(double value)
{
    facingAngle=value;
    return this;
}

Entity* Entity::setForceFacing(bool forced, bool refreshFacing)
{
    forceFacing=forced;
    if(refreshFacing)
    {
        if(forced)
            setFacingAngle(rotation);
        else
            setFacingAngle(0.0);
    }
    return this;
}

Entity* Entity::setPositionOffset(double offsetX, double offsetY)
{
    x+=offsetX;
    y+=offsetY;
    return this;
}

Entity* Entity::setSpeedOffset(double offsetVel)
{
    setSpeed(speed+offsetVel);
    return this;
}

Entity* Entity::setRotationOffset(double offsetAngle)
{
    setRotation(rotation+offsetAngle);
    return this;
}

Entity* Entity::setVelocityOffset(double offsetVel, double offsetAngle)
{
    setAxisSpeedOffset(offsetVel*sin(offsetAngle),offsetVel*cos(offsetAngle));
    return this;
}

Entity* Entity::setAxisSpeedOffset(double offsetVelX, double offsetVelY)
{
    correctAxisSpeed();
    speedX+=offsetVelX;
    speedY+=offsetVelY;
    correctVelocityAngle();
    return this;
}

Entity* Entity::offsetMotion(double ticks)
{
    if(!useDefaultMotion)return this;
    correctAxisSpeed();
    x+=speedX*ticks;
    y+=speedY*ticks;
    return this;
}

void Entity::correctAxisSpeed()
{
    if(isAxisVelocityCorrect)return;
    speedX=speed*sin(rotation);
    speedY=speed*cos(rotation);
    isAxisVelocityCorrect=true;
}

void Entity::correctVelocityAngle()
{
    if(isAxisVelocityCorrect)return;
    setSpeed(dist(speedX, speedY));
    if(speed>M_INFS)
        setRotation(atan2(speedX, speedY));
    isAxisVelocityCorrect=true;
}

//===================================================

Entity::Entity()
    :Ticking()
    ,Position()
    ,speed(0.0)
    ,rotation(0.0)
    ,speedX(0.0)
    ,speedY(0.0)
    ,facingAngle(0.0)
    ,forceFacing(true)
    ,useDefaultMotion(true)
    ,useDefaultRendering(true)
    ,isAxisVelocityCorrect(true)
{

}

Entity::Entity(double posX, double posY)
    :Ticking()
    ,Position(posX, posY)
    ,speed(0.0)
    ,rotation(0.0)
    ,speedX(0.0)
    ,speedY(0.0)
    ,facingAngle(0.0)
    ,forceFacing(true)
    ,useDefaultMotion(true)
    ,useDefaultRendering(true)
    ,isAxisVelocityCorrect(true)
{
}

Entity::Entity(double posX, double posY, double vel, double angle)
    :Entity(posX, posY)
{
    setSpeedRotation(vel, angle);
}

Entity::~Entity()
{

}

//===================================================

void Entity::onUpdateMotion(double, double deltaTick)
{
    offsetMotion(deltaTick);
}

void Entity::onTick(){}

void Entity::onRender()
{
    if(!useDefaultRendering)
        return;
    d3d.pushVertex(0.0, 0.0, 0.0, 0.0);
    d3d.pushVertex(1.0, 0.0, 1.0, 0.0);
    d3d.pushVertex(1.0, 1.0, 1.0, 1.0);
    d3d.pushVertex(0.0, 1.0, 0.0, 1.0);
}

void Entity::destroy()
{
    setDead();
}

void Entity::render()
{
    if(isDead)return;
    d3d.pushMatrix();
    if(useDefaultRendering)
    {
        d3d.translate2D(x, y);
        d3d.rotate2D(-facingAngle);
    }
    onRender();
    d3d.popMatrix();
}
