#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "texturestrip.h"
#include "keybinding.h"

enum PlayerDirection
{
    Dir_None = 0,
    Dir_Left = 0x1,
    Dir_Up = 0x2,
    Dir_Right = 0x4,
    Dir_Down = 0x8,
    Dir_LeftUp = Dir_Left|Dir_Up,
    Dir_LeftDown = Dir_Left|Dir_Down,
    Dir_RightUp = Dir_Right|Dir_Up,
    Dir_RightDown = Dir_Right|Dir_Down,
    Dir_All = 0xF
};

class BulletList;

class Player : public Entity
{
public:

    Player();

    void setJudgeList(BulletList* list);

    void setMoveSpeed(double speed);
    void setMoveField(double xmin, double xmax, double ymin, double ymax);

    void setDirection(PlayerDirection dir);

    inline double getJudgeSpan() const
    {
        return judgeSpan;
    }

protected:

    TextureStrip tex;

    BulletList* judgeList;

    double judgeSpan;
    double moveSpeed;

    double lastMissTime;
    double invinTime;

    double minX, maxX;
    double minY, maxY;

    void onUpdateMotion(double deltaSec, double deltaTick);
    void onTick();
    void onRender();
    void onDestroy();

    friend class BulletJudge;

};

#endif // PLAYER_H
