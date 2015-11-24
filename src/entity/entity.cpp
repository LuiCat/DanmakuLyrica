#include "entity.h"

#include "mathhelper.h"

#include "debug.h"

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

Entity& Entity::setSpeed(double value)
{
    speed=value;
    if(speed<0.0)
    {
        speed=0;
        //speed=-speed;
        //rotation+=M_PI;
    }
    isAxisVelocityCorrect=false;
    return *this;
}

Entity& Entity::setRotation(double value)
{
    rotation=value;
    isAxisVelocityCorrect=false;
    if(forceFacing)
        setFacingAngle(rotation);
    return *this;
}

Entity& Entity::setSpeedRotation(double vel, double angle)
{
    setSpeed(vel);
    setRotation(angle);
    correctAxisSpeed();
    return *this;
}

Entity& Entity::setSpeedX(double velX)
{
    speedX=velX;
    correctVelocityAngle();
    return *this;
}

Entity& Entity::setSpeedY(double velY)
{
    speedY=velY;
    correctVelocityAngle();
    return *this;
}

Entity& Entity::setAxisSpeed(double velX, double velY)
{
    speedX=velX;
    speedY=velY;
    correctVelocityAngle();
    return *this;
}

Entity& Entity::setFacingAngle(double value)
{
    facingAngle=value;
    return *this;
}

Entity& Entity::setForceFacing(bool forced, bool refreshFacing)
{
    forceFacing=forced;
    if(refreshFacing)
    {
        if(forced)
            setFacingAngle(rotation);
        else
            setFacingAngle(0.0);
    }
    return *this;
}

Entity& Entity::setPositionOffset(double offsetX, double offsetY)
{
    x+=offsetX;
    y+=offsetY;
    return *this;
}

Entity& Entity::setSpeedOffset(double offsetVel)
{
    setSpeed(speed+offsetVel);
    return *this;
}

Entity& Entity::setRotationOffset(double offsetAngle)
{
    setRotation(rotation+offsetAngle);
    return *this;
}

Entity& Entity::setVelocityOffset(double offsetVel, double offsetAngle)
{
    setAxisSpeedOffset(offsetVel*sin(offsetAngle),offsetVel*cos(offsetAngle));
    return *this;
}

Entity& Entity::setAxisSpeedOffset(double offsetVelX, double offsetVelY)
{
    correctAxisSpeed();
    speedX+=offsetVelX;
    speedY+=offsetVelY;
    isAxisVelocityCorrect=false;
    correctVelocityAngle();
    return *this;
}

double Entity::getAccelerateSpeed() const
{
    return accelerateSpeed;
}

double Entity::getRotateSpeed() const
{
    return rotateSpeed;
}

Entity& Entity::setAccelerateSpeed(double accVel)
{
    accelerateSpeed=accVel;
    return *this;
}

Entity& Entity::setRotateSpeed(double rotateVel)
{
    rotateSpeed=rotateVel;
    return *this;
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
    if(speed>M_DINFS)
        setRotation(atan2(speedX, speedY));
    isAxisVelocityCorrect=true;
}

//===================================================

Entity& Entity::offsetMotion(double secs)
{
    if(!useDefaultMotion)return *this;
    if(rotateSpeed<M_DINFS&&rotateSpeed>-M_DINFS)
    {
        if(accelerateSpeed<M_DINFS&&accelerateSpeed>-M_DINFS) // uniform linear motion
        {
            correctAxisSpeed();
            x+=speedX*secs;
            y+=speedY*secs;
        }
        else // accelerated linear motion
        {
            setSpeed(getSpeed()+0.5*accelerateSpeed*secs);
            correctAxisSpeed();
            x+=speedX*secs;
            y+=speedY*secs;
            setSpeed(getSpeed()+0.5*accelerateSpeed*secs);
        }
    }
    else // accelerated rotate motion, go along involute path
    {
        double oldSpeed=getSpeed();
        double oldRot=getRotation();
        double newSpeed=oldSpeed+accelerateSpeed*secs;
        double newRot=oldRot+rotateSpeed*secs;
        setSpeedRotation(newSpeed, newRot);
        if(newSpeed<0.0)
        {
            newSpeed=0;
            newRot=oldRot+rotateSpeed*oldSpeed/accelerateSpeed;
        }

        double ra=accelerateSpeed/rotateSpeed/rotateSpeed;
        double oldcos=cos(oldRot), oldsin=sin(oldRot);
        double newcos=cos(newRot), newsin=sin(newRot);

        x+=(oldSpeed*oldcos-newSpeed*newcos)/rotateSpeed+ra*(newsin-oldsin);
        y+=(newSpeed*newsin-oldSpeed*oldsin)/rotateSpeed+ra*(newcos-oldcos);

    }
    return *this;
}

//===================================================

Entity::Entity()
    :Sprite()
    ,speed(0.0)
    ,rotation(0.0)
    ,speedX(0.0)
    ,speedY(0.0)
    ,accelerateSpeed(0.0)
    ,rotateSpeed(0.0)
    ,facingAngle(0.0)
    ,forceFacing(true)
    ,useDefaultMotion(true)
    ,isAxisVelocityCorrect(true)
{

}

Entity::Entity(double posX, double posY)
    :Sprite(posX, posY)
    ,speed(0.0)
    ,rotation(0.0)
    ,speedX(0.0)
    ,speedY(0.0)
    ,accelerateSpeed(0.0)
    ,rotateSpeed(0.0)
    ,facingAngle(0.0)
    ,forceFacing(true)
    ,useDefaultMotion(true)
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

void Entity::onUpdateMotion(double deltaSec, double)
{
    EntityEvent* p;
    while(!eventList.empty())
    {
        p=eventList.top();
        deltaSec=p->process(this, deltaSec);
        if(!p->isProcessed())
            break;
        eventList.pop();
    }
    offsetMotion(deltaSec);
}

void Entity::onTick(){}

void Entity::onRender()
{
    d3d.vertex(0.0, 0.0, 0.0, 0.0);
    d3d.vertex(1.0, 0.0, 1.0, 0.0);
    d3d.vertex(1.0, 1.0, 1.0, 1.0);
    d3d.vertex(0.0, 1.0, 0.0, 1.0);
}

void Entity::onDestory()
{
    setDead();
}

//===================================================

void Entity::destroy()
{
    onDestory();
}

void Entity::render()
{
    if(isDead)return;
    d3d.pushMatrix();
    if(fabs(facingAngle)<M_DINFS)
        d3d.translate2D(round(x), round(y));
    else
        d3d.translate2D(x, y);
    d3d.rotate2D(-facingAngle);
    onRender();
    d3d.popMatrix();
}
