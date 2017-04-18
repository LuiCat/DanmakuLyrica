#ifndef SPRITE_H
#define SPRITE_H

#include "entity.h"
#include "texturepiece.h"

class Sprite : public Entity
{
protected:

    TexturePiece image;
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

    Sprite& setImage(TexturePiece&& newImage);
    const TexturePiece& getImage() const;

    double getAlpha() const;
    Sprite& setAlpha(double x);

    Sprite();
    Sprite(double x, double y);

};

#endif // SPRITE_H
