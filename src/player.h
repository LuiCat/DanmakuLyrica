#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "texturestrip.h"
#include "keybinding.h"

enum PlayerDirection
{
    Dir_None = 0,
    Dir_Left = 0x001,
    Dir_Right = 0x101,
    Dir_Up = 0x0010,
    Dir_Down = 0x1010,
    Dir_LeftUp = Dir_Left|Dir_Up,
    Dir_LeftDown = Dir_Left|Dir_Down,
    Dir_RightUp = Dir_Right|Dir_Up,
    Dir_RightDown = Dir_Right|Dir_Down,
    Dir_Mask_Move = 0x1111
};

class BulletList;

class Player : public Entity
{
public:

    Player();

    void setJudgeList(BulletList* list);

    void setMoveSpeed(double speed);
    void setDirection(PlayerDirection dir);

    inline double getJudgeSpan() const
    {
        return judgeSpan;
    }

protected:

    TextureStrip tex;

    BulletList* judgeList;

    double moveSpeed;
    double judgeSpan;

    void onUpdateMotion(double deltaSec, double deltaTick);
    void onTick();
    void onRender();
    void onDestory();

    friend class BulletJudge;

};

#endif // PLAYER_H
