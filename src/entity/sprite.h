#ifndef SPRITE_H
#define SPRITE_H

#include "entity.h"
#include "texturestrip.h"

class Sprite : public Entity
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

    Sprite& setTexture(Texture tex);
    Sprite& setRenderSize(double x, double y);
    Sprite& setRenderCenter(double x, double y);
    Sprite& setTextureSize(double posX, double posY, double sizeX, double sizeY);

    double getAlpha() const;
    Sprite& setAlpha(double x);

    Sprite();
    Sprite(double x, double y);

};

#endif // SPRITE_H
