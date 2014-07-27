#include "object.h"

//===================================================

Texture Object::getTexture() const
{
    return texture;
}

Object* Object::setTexture(Texture tex)
{
    texture=tex;
    return this;
}

Object* Object::setRenderSize(double x, double y)
{
    renderSizeX=x;
    renderSizeY=y;
    return this;
}

Object* Object::setRenderCenter(double x, double y)
{
    renderCenterX=x;
    renderCenterY=y;
    return this;
}

Object* Object::setTextureSize(double posX, double posY, double sizeX, double sizeY)
{
    texturePosX=posX;
    texturePosY=posY;
    textureSizeX=sizeX;
    textureSizeY=sizeY;
    return this;
}

double Object::getAlpha() const
{
    return useAlpha?x:1.0;
}

Object* Object::setAlpha(double x)
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
    return this;
}

//===================================================

Object::Object()
    :Entity()
    ,texture(0)
    ,renderSizeX(16.0)
    ,renderSizeY(16.0)
    ,renderCenterX(8.0)
    ,renderCenterY(8.0)
    ,texturePosX(0.0)
    ,texturePosY(0.0)
    ,textureSizeX(1.0)
    ,textureSizeY(1.0)
    ,useAlpha(false)
    ,alpha(1.0)
{
}

Object::Object(double x, double y)
    :Entity(x, y)
    ,texture(0)
    ,renderSizeX(16.0)
    ,renderSizeY(16.0)
    ,renderCenterX(8.0)
    ,renderCenterY(8.0)
    ,texturePosX(0.0)
    ,texturePosY(0.0)
    ,textureSizeX(1.0)
    ,textureSizeY(1.0)
    ,useAlpha(false)
    ,alpha(1.0)
{
}

void Object::onTick()
{
}

void Object::onRender()
{
    d3d.pushMatrix();
    d3d.translate(-renderCenterX, -renderCenterY);
    d3d.scale(renderSizeX, renderSizeY);
    d3d.setTexture(texture);
    if(useAlpha)
        d3d.setAlpha(alpha);
    d3d.pushVertex(0.0, 0.0, 0.0, 0.0);
    d3d.pushVertex(1.0, 0.0, 1.0, 0.0);
    d3d.pushVertex(1.0, 1.0, 1.0, 1.0);
    d3d.pushVertex(0.0, 1.0, 0.0, 1.0);
    d3d.popMatrix();
}
