#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "rendersurface.h"

class RenderTexture : public RenderTarget
{
private:

    Texture texture;
    Surface texSurface;

public:

    RenderTexture();
    RenderTexture(int width, int height);
    ~RenderTexture();

    void pushVertices();
    void draw(int x, int y, RenderTarget* target=0);

    Surface surface();

    inline operator Texture() const
    {
        return texture;
    }

    inline Texture operator()() const
    {
        return texture;
    }

};

#endif // RENDERTEXTURE_H
