#include "texturestrip.h"

TextureStrip::TextureStrip(Texture tex)
    :posX(0.0)
    ,posY(0.0)
    ,sizeX(1.0)
    ,sizeY(1.0)
    ,maxIndexX(1)
    ,maxIndexY(1)
{
    setTexture(tex);
}

void TextureStrip::setTexture(Texture tex)
{
    texture=tex;
}

void TextureStrip::setStripPos(double x, double y, double w, double h)
{
    posX=x;
    posY=y;
    sizeX=w;
    sizeY=h;
}

void TextureStrip::setMaxIndex(int i, int j)
{
    maxIndexX=j;
    maxIndexY=i;
}

void TextureStrip::pushVertices(int i, int j)
{
    double umin, vmin, umax, vmax;

    umin=posX+(maxIndexX>0?j%maxIndexX:j)*sizeX;
    vmin=posY+(maxIndexY>0?i%maxIndexY:i)*sizeY;
    umax=umin+sizeX;
    vmax=vmin+sizeY;

    d3d.setTexture(texture);

    d3d.pushVertex(0.0, 0.0, umin, vmin);
    d3d.pushVertex(1.0, 0.0, umax, vmin);
    d3d.pushVertex(1.0, 1.0, umax, vmax);
    d3d.pushVertex(0.0, 1.0, umin, vmax);
}
