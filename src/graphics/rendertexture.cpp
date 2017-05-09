#include "rendertexture.h"

#include "shape2d.h"
#include "commondef.h"

RenderTexture::RenderTexture()
    :RenderTexture(WIDTH, HEIGHT)
{

}

RenderTexture::RenderTexture(int width, int height)
    :RenderTarget(width, height)
    ,texture(0)
    ,texSurface(0)
{
    pD3DDevice->CreateTexture(width, height, 0, D3DUSAGE_RENDERTARGET,
                              D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, 0);
    texture->GetSurfaceLevel(0, &texSurface);
}

RenderTexture::~RenderTexture()
{
    texture->Release();
}

void RenderTexture::pushVertices()
{
    d2d.setTexture(texture);
    d2d.rect(0, 0, 1, 1);
}

void RenderTexture::draw(int x, int y, RenderTarget* target)
{
    d2d.beginScene();
    d2d.resetMatrix();
    d2d.translate(x, y);
    d2d.scale(width, height);
    pushVertices();
    d2d.drawScene(target);
    d2d.endScene();
}

Surface RenderTexture::surface()
{
    return texSurface;
}
