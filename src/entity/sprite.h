#ifndef SPRITE_H
#define SPRITE_H

#include "graphics.h"

#include "position.h"
#include "beatticking.h"

class Sprite : public Ticking, public Position
{
public:

    Sprite();
    Sprite(double posX, double posY);

    virtual void render();

protected:

    virtual void onRender();

};

#endif // SPRITE_H
