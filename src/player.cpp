#include "player.h"

#include "bulletlist.h"
#include "commondef.h"
#include "debug.h"
#include "soundregistry.h"

Player::Player()
    : Entity(0, 0)
    , judgeList(0)
    , judgeSpan(0.0)
    , moveSpeed(160.0)
    , lastMissTime(0.0)
    , invinTime(0.5)
    , minX(-WIDTH/2)
    , maxX(WIDTH/2)
    , minY(-HEIGHT/2)
    , maxY(HEIGHT/2)
{
    setTickRate(120);
    tex.loadFile("data/image/player/test.png");
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
    // remove conflicted directions by xor shifted dirs
    dir = (PlayerDirection)(dir&(~((dir<<2)|(dir>>2)))&Dir_All);
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

void Player::setMoveField(double xmin, double xmax, double ymin, double ymax)
{
    minX = xmin;
    maxX = xmax;
    minY = ymin;
    maxY = ymax;
}

void Player::onUpdateMotion(double deltaSec, double deltaTick)
{
    // do some record ?
    Entity::onUpdateMotion(deltaSec, deltaTick);
    if(x < minX) setX(minX);
    else if(x > maxX) setX(maxX);
    if(y < minY) setY(minY);
    else if(y > maxY) setY(maxY);
}

void Player::onTick()
{
    static int t = 0;
    // do some record ?
    if(!judgeList)
        return;
    if(judgeList->judgeBullets(*this))
    {
        if(timeSec-lastMissTime>invinTime)
        {
            lastMissTime = getTimeSec();
            cout<<"miss "<<(++t)<<endl;
            SOUND("miss")->play(true);
        }
    }
}

void Player::onRender()
{
    d3d.pushMatrix();
    d3d.translate2D(-30, -30);
    d3d.scale2D(60, 60);
    tex.pushVertices(0, 0);
    d3d.popMatrix();
}

void Player::onDestroy()
{
    // do some magic ?
}
