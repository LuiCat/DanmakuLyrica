#include "player.h"

#include "bulletlist.h"
#include "commondef.h"
#include "debug.h"
#include "soundregistry.h"
#include "mathhelper.h"
#include "notescene.h"

Player::Player()
    : Entity(0, 100)
    , judgeList(nullptr)
    , judgeSpan(0.0)
    , moveSpeed(160.0)
    , lastMissTime(0.0)
    , invinTime(0.5)
    , minX(-WIDTH/2)
    , maxX(WIDTH/2)
    , minY(-HEIGHT/2)
    , maxY(HEIGHT/2)
    , moveFrame(0)
    , stayFrame(0)
    , lastHitTime(-M_DINF)
    , targetPos(0, -1000)
    , target(nullptr)
    , noteScene(nullptr)
{
    setTickRate(120);
    setForceFacing(false);

    tex.load("data/image/player/judgepoint.png");
    img = TexturePiece::createImageSet("data/image/player/lyrica.png", 2, 4, 8);
    plotInner.load("data/image/player/plot_inner.png");
    plotOuter.load("data/image/player/plot_outer.png");

    number.loadImage("data/image/number/num.png", 3, 4);
    number.setAlignment(NumberSprite::AlignCenter);
    number.setCharSize(60, 45, 0.6);
    number.setNumber(0);

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

void Player::setTarget(SpiritWheel* spirit)
{
    target = spirit;
    if(spirit == nullptr)
    {
        targetPos.setPosition(0, -1000);
    }
    else
    {
        targetPos = *spirit;
    }
}

void Player::onHitTarget()
{
    lastHitTime = timeSec;
}

void Player::checkTargetDist()
{
    if(target != nullptr)
    {
        if(target->destroyed() || target->distanceTo(*this) > 200)
        {
            target=nullptr;
            targetPos.setPosition(0, -1000);
        }
        else
        {
            targetPos = *target;
        }
    }
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

    double sx = getSpeedX();
    if(sx < -1)
    {
        moveFrame+=1;
    }
    else if(sx > 1)
    {
        moveFrame-=1;
    }
    else if(moveFrame != 0)
    {
        moveFrame+=(moveFrame>0?-1:1);
    }
    else
    {
        stayFrame+=1;
        if(stayFrame>=16)
            stayFrame=0;
    }
    if(moveFrame>8)
        moveFrame=8;
    if(moveFrame<-8)
        moveFrame=-8;

    // do some record ?

    if(judgeList!=nullptr && judgeList->judgeBullets(*this))
    {
        if(timeSec-lastMissTime>invinTime)
        {
            lastMissTime = timeSec;
            if(noteScene!=nullptr)
                noteScene->breakCombo();
            score->addRawScore(1);
            cout<<"miss "<<(++t)<<endl;
            SOUND("miss")->play(true);
        }
    }
}

void Player::onRender()
{
    if(moveFrame > 2)
        img[(2+moveFrame)/5+5].vertice(0.5, 0.5, 96);
    else if(moveFrame < -2)
        img[(2-moveFrame)/5+3].vertice(0.5, 0.5, 96);
    else
        img[stayFrame/4].vertice(0.5, 0.5, 96);
}

void Player::renderOverlay()
{
    d2d.pushMatrix();

    d2d.translate(x, y);
    tex.vertice(0.5, 0.5, 60);

    double dx = targetPos.getX()-x;
    double dy = targetPos.getY()-y;
    double dis = dist(dx, dy);
    double angle = atan2(dx, dy);
    double rot = 0;
    double size = 128;
    bool isTargeted = false;

    d2d.pushMatrix();

    if(dis<200)
    {
        isTargeted = true;
        d2d.translate(dx, dy);
        rot = timeSec*M_PI_2;
        size *= 1-0.25*exp(-5*(timeSec-lastHitTime));
    }
    else
    {
        rot = timeSec*M_PI_2*0.2;
        dis = 50+150*exp((200-dis)*0.01);
        d2d.translate(dis*sin(angle), dis*cos(angle));
        d2d.setAlpha(0.5);
        size = 80;
    }

    d2d.pushMatrix();
    d2d.rotate(rot);
    plotInner.vertice(0.5, 0.5, size);
    d2d.popMatrix();

    d2d.pushMatrix();
    d2d.rotate(-rot);
    plotOuter.vertice(0.5, 0.5, size);
    d2d.popMatrix();

    if(isTargeted && target!=nullptr)
    {
        number.setNumber(target->remainHit());
        number.render();
    }

    d2d.popMatrix();

    d2d.popMatrix();
}

void Player::setNoteScene(NoteScene* scene)
{
    noteScene = scene;
}

void Player::setScoreObj(Score* score)
{
    this->score = score;
}

void Player::onDestroy()
{
    // do some magic ?
}
