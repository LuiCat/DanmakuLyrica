#ifndef TEXTURESTRIP_H
#define TEXTURESTRIP_H

#include "graphics.h"

class TextureStrip
{
public:

    TextureStrip(Texture tex=0);

    void setTexture(Texture tex);
    void setStripPos(double x, double y, double w, double h);
    void setMaxIndex(int i, int j);

    void pushVertices(int i, int j);

protected:

    Texture texture;

    double posX, posY;
    double sizeX, sizeY;
    int maxIndexX, maxIndexY;

};

#endif // TEXTURESTRIP_H
