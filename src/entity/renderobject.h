#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "graphics.h"

#include "position.h"
#include "beatticking.h"

class RenderObject : public Ticking, public Position
{
public:

    RenderObject();
    RenderObject(double posX, double posY);

    virtual void render();

protected:

    virtual void onRender();

};

#endif // RENDEROBJECT_H
