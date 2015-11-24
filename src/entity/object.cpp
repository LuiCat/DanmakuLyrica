#include "object.h"

//===================================================

Object& Object::setTexture(Texture tex)
{
    textureStrip.setTexture(tex);
    return *this;
}

Object& Object::setRenderSize(double x, double y)
{
    renderSizeX=x;
    renderSizeY=y;
    return *this;
}

Object& Object::setRenderCenter(double x, double y)
{
    renderCenterX=x;
    renderCenterY=y;
    return *this;
}

Object& Object::setTextureSize(double posX, double posY, double sizeX, double sizeY)
{
    textureStrip.setStripPos(posX, posY, sizeX, sizeY);
    return *this;
}

double Object::getAlpha() const
{
    return useAlpha?x:1.0;
}

Object& Object::setAlpha(double x)
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

Object::Object()
    :Entity()
    ,textureStrip(0)
    ,renderSizeX(16.0)
    ,renderSizeY(16.0)
    ,renderCenterX(8.0)
    ,renderCenterY(8.0)
    ,useAlpha(false)
    ,alpha(1.0)
{
}

Object::Object(double x, double y)
    :Entity(x, y)
    ,textureStrip(0)
    ,renderSizeX(16.0)
    ,renderSizeY(16.0)
    ,renderCenterX(8.0)
    ,renderCenterY(8.0)
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
    d3d.translate2D(-renderCenterX, -renderCenterY);
    d3d.scale2D(renderSizeX, renderSizeY);
    if(useAlpha)
        d3d.setAlpha(alpha);
    textureStrip.pushVertices(0, 0);
    d3d.popMatrix();
}
