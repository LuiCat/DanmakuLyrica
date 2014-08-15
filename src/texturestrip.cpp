#include "texturestrip.h"

TextureStrip::TextureStrip(Texture tex)
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

    umin=posX+j%maxIndexX*sizeX;
    vmin=posY+i%maxIndexY*sizeY;
    umax=umin+sizeX;
    vmax=vmin+sizeY;

    d3d.pushVertex(0.0, 0.0, umin, vmin);
    d3d.pushVertex(1.0, 0.0, umax, vmin);
    d3d.pushVertex(1.0, 1.0, umax, vmax);
    d3d.pushVertex(0.0, 1.0, umin, vmax);
}
