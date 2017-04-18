#ifndef SPIRITWHEEL_H
#define SPIRITWHEEL_H

#include "bulletbase.h"
#include "texturepiece.h"

class SpiritWheel : public BulletBase
{
protected:

    static TexturePiece imgWheel;

    double life;

    int maxHp;
    int currHit;

public:

    SpiritWheel(double posX, double posY, double vel, double angle, double life, int hp);

    virtual ExtendType getType() const
    {
        return Type_SpiritWheel;
    }

    bool hit(int times = 1);

    inline int remainHit() const
    {
        return maxHp-currHit;
    }

    void onTick();
    void onRender();
    void onDestroy();

    bool isOutsideScene(double x1, double y1, double x2, double y2);


};

#endif // SPIRITWHEEL_H
