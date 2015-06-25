#include "textrenderer.h"

#include "commondef.h"

TextRenderer::TextRenderer(int height, int width, int weight, bool italic, const char* font)
    :lpfont(0)
    ,height(height)
    ,width(width)
    ,weight(weight)
    ,italic(italic)
{
    D3DXCreateFont(pD3DDevice, height, width, weight, D3DX_DEFAULT, false,
                   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, font, &lpfont);
    clear();
}

void TextRenderer::clear()
{
    ss.str("");
}

void TextRenderer::present(UINT format, D3DCOLOR color)
{
    if(SUCCEEDED(pD3DDevice->BeginScene()))
    {
        RECT rect={0, 0, WIDTH, HEIGHT};
        int res=lpfont->DrawText(0, ss.str().c_str(), -1, &rect, format, color);
        if(res==0)
            res=1;
        pD3DDevice->EndScene();
    }
    clear();
}

HRESULT TextRenderer::lost()
{
    return lpfont->OnLostDevice();
}

HRESULT TextRenderer::reset()
{
    return lpfont->OnResetDevice();
}

