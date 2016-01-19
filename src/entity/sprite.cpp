#include "sprite.h"

//===================================================

Sprite& Sprite::setTexture(Texture tex)
{
    image.setTexture(tex);
    return *this;
}

Sprite& Sprite::setRenderSize(double x, double y)
{
    renderSizeX=x;
    renderSizeY=y;
    return *this;
}

Sprite& Sprite::setRenderCenter(double x, double y)
{
    renderCenterX=x;
    renderCenterY=y;
    return *this;
}

Sprite& Sprite::setImage(ImagePiece&& newImage)
{
    image = std::forward<ImagePiece>(newImage);
    return *this;
}

const ImagePiece& Sprite::getImage() const
{
    return image;
}

double Sprite::getAlpha() const
{
    return useAlpha?x:1.0;
}

Sprite& Sprite::setAlpha(double x)
{
    if(x<0.0)
    {
        useAlpha=false;
    }
    else
    {
        useAlpha=true;
        if(x>1.0)x=1.0;
        alpha=x;
    }
    return *this;
}

//===================================================

Sprite::Sprite()
    :Entity()
    ,renderSizeX(16.0)
    ,renderSizeY(16.0)
    ,renderCenterX(8.0)
    ,renderCenterY(8.0)
    ,useAlpha(false)
    ,alpha(1.0)
{
}

Sprite::Sprite(double x, double y)
    :Entity(x, y)
    ,renderSizeX(16.0)
    ,renderSizeY(16.0)
    ,renderCenterX(8.0)
    ,renderCenterY(8.0)
    ,useAlpha(false)
    ,alpha(1.0)
{
}

void Sprite::onTick()
{
}

void Sprite::onRender()
{
    d3d.pushMatrix();
    d3d.translate2D(-renderCenterX, -renderCenterY);
    d3d.scale2D(renderSizeX, renderSizeY);
    if(useAlpha)
        d3d.setAlpha(alpha);
    image.vertice();
    d3d.popMatrix();
}
