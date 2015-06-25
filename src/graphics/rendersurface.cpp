#include "rendersurface.h"

#include "commondef.h"

RenderSurface::RenderSurface()
    :RenderSurface(WIDTH, HEIGHT)
{

}

RenderSurface::RenderSurface(int width, int height)
    :RenderTarget(width, height)
    ,pD3DSurface(0)
{
    pD3DDevice->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8,
                                   d3dpp.MultiSampleType, d3dpp.MultiSampleQuality,
                                   FALSE, &pD3DSurface, NULL);
}

RenderSurface::~RenderSurface()
{
    if(pD3DSurface)
        pD3DSurface->Release();
    pD3DSurface=0;
}

Surface RenderSurface::surface()
{
    return pD3DSurface;
}
