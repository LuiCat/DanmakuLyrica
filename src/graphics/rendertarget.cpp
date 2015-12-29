#include "rendertarget.h"


RenderTarget::RenderTarget(int width, int height)
    :lastSurface(0)
    ,isTarget(false)
    ,width(width)
    ,height(height)
{

}

void RenderTarget::clear()
{
    if(isTarget)
    {
        pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);
    }
    else
    {
        setTarget();
        pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);
        unsetTarget();
    }
}

void RenderTarget::setTarget()
{
    if(!isTarget)
    {
        pD3DDevice->GetRenderTarget(0, &lastSurface);
        pD3DDevice->SetRenderTarget(0, surface());
        isTarget=true;
        clear();
    }
}

void RenderTarget::unsetTarget()
{
    if(isTarget)
    {
        pD3DDevice->SetRenderTarget(0, lastSurface);
        lastSurface=0;
        isTarget=false;
    }
}

HRESULT RenderTarget::present(int x, int y, RenderTarget* target)
{
    Surface dest;
    RECT sr={0, 0, width, height};
    RECT sd={x, y, x+width, y+height};

    if(target)
    {
        dest=target->surface();
    }
    else
    {
        pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &dest);
    }

    return pD3DDevice->StretchRect(surface(), &sr, dest, &sd, D3DTEXF_LINEAR);
}
