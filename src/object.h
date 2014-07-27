#ifndef OBJECT_H
#define OBJECT_H

#include "entity.h"

class Object : public Entity
{
protected:

    Texture texture;
    double renderSizeX, renderSizeY;
    double renderCenterX, renderCenterY;
    double texturePosX, texturePosY;
    double textureSizeX, textureSizeY;

    bool useAlpha;
    double alpha;

    void onTick();
    void onRender();

public:

    Texture getTexture() const;
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
