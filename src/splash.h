#ifndef SPLASH_H
#define SPLASH_H

#include "bulletbase.h"

#include <texturepiece.h>


class Splash : public BulletBase
{
public:

    static TexturePiece img;

    Splash(double x, double y);

    ExtendType getType() const
    {
        return Type_Effect;
    }

protected:

    bool isOutsideScene(double x1, double y1, double x2, double y2);

    void onTick();
    void onRender();

};

#endif // SPLASH_H
