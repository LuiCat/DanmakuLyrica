#include "rendertexture.h"

#include "vertexbuffer.h"
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
    d3d.setTexture(texture);
    d3d.rect(0, 0, 1, 1);
}

void RenderTexture::draw(int x, int y, RenderTarget* target)
{
    d3d.beginScene(target);
    d3d.translate2D(x, y);
    d3d.scale2D(width, height);
    pushVertices();
    d3d.endScene();
}

Surface RenderTexture::surface()
{
    return texSurface;
}
