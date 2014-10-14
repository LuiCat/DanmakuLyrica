#ifndef OBJECT_H
#define OBJECT_H

#include "entity.h"
#include "texturestrip.h"

class Object : public Entity
{
protected:

    TextureStrip textureStrip;
    double renderSizeX, renderSizeY;
    double renderCenterX, renderCenterY;

    bool useAlpha;
    double alpha;

    void onTick();
    void onRender();

public:

    Object* setTexture(Texture tex);
    Object* setRenderSize(double x, double y);
    Object* setRenderCenter(double x, double y);
    Object* setTextureSize(double posX, double posY, double sizeX, double sizeY);

    double getAlpha() const;
    Object* setAlpha(double x);

    Object();
    Object(double x, double y);

};

#endif // OBJECT_H
