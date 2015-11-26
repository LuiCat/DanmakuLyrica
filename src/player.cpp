#include "player.h"

#include "bulletlist.h"
#include "commondef.h"

Player::Player()
    : Entity(0, 0)
{

}

void Player::setJudgeList(BulletList* list)
{
    judgeList = list;
}

void Player::setMoveSpeed(double speed)
{
    moveSpeed = speed;
}

void Player::setDirection(PlayerDirection dir)
{
    dir = (PlayerDirection)(dir&(dir<<2)&Dir_Mask_Move);
    double rotm;

    switch(dir)
    {
    case Dir_Down:
        rotm = 0.0;
        break;
    case Dir_RightDown:
        rotm = 0.25;
        break;
    case Dir_Right:
        rotm = 0.5;
        break;
    case Dir_RightUp:
        rotm = 0.75;
        break;
    case Dir_Up:
        rotm = 1.0;
        break;
    case Dir_LeftUp:
        rotm = 1.25;
        break;
    case Dir_Left:
        rotm = 1.5;
        break;
    case Dir_LeftDown:
        rotm = 1.75;
        break;
    default:
        setSpeed(0);
        return;
    }

    setSpeedRotation(moveSpeed, rotm*M_PI);

}

void Player::onUpdateMotion(double deltaSec, double deltaTick)
{
    // do some record ?
    Entity::onUpdateMotion(deltaSec, deltaTick);
}

void Player::onTick()
{
    // do some record ?
    if(judgeList)
    {
        judgeList->judgeBullets(*this);
    }
}

void Player::onRender()
{
    d3d.pushMatrix();
    d3d.translate2D(30, 30);
    d3d.scale2D(60, 60);
    tex.pushVertices(0, 0);
    d3d.popMatrix();
}

void Player::onDestory()
{
    // do some magic ?
}
