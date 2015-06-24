#include "textrenderer.h"

#include "gfxprivate.h"
#include "commondef.h"

TextRenderer::TextRenderer(int height, int width, int weight, bool italic, const char* font)
    :lpfont(0)
    ,height(height)
    ,width(width)
    ,weight(weight)
    ,italic(italic)
{
    D3DXCreateFont(g_pd3dDevice, height, width, weight, D3DX_DEFAULT, false,
                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, font, &lpfont);
    reset();
}

void TextRenderer::reset()
{
    ss.str("");
}

void TextRenderer::present(UINT format, D3DCOLOR color)
{
    if(SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        RECT rect={0, 0, WIDTH, HEIGHT};
        int res=lpfont->DrawText(0, ss.str().c_str(), -1, &rect, format, color);
        if(res==0)
            res=1;
        g_pd3dDevice->EndScene();
    }
    reset();
}

HRESULT TextRenderer::onLost()
{
    return lpfont->OnLostDevice();
}

HRESULT TextRenderer::onReset()
{
    return lpfont->OnResetDevice();
}

