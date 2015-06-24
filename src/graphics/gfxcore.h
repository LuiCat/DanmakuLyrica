#ifndef GFXCORE_H
#define GFXCORE_H

#include "gfxdef.h"

class GFXCore
{
private:

    static void setDeviceStates();

protected:

    static LPDIRECT3D9 g_pd3d;
    static LPDIRECT3DDEVICE9 g_pd3dDevice;
    static D3DPRESENT_PARAMETERS   d3dpp;

public:

    static DWORD createTexture(const char *filename, Texture* tex, DWORD w=D3DX_DEFAULT, DWORD h=D3DX_DEFAULT);

    static HRESULT initDevice(HWND hWnd);
    static void cleanupDevice();

    static HRESULT restoreDevice();
    static HRESULT resetDevice();

};

inline DWORD createTexture(const char *filename, Texture* tex, DWORD w=D3DX_DEFAULT, DWORD h=D3DX_DEFAULT)
{
    return GFXCore::createTexture(filename, tex, w, h);
}

inline HRESULT D3D_InitDevice(HWND hWnd)
{
    return GFXCore::initDevice(hWnd);
}

inline void D3D_CleanupDevice()
{
    GFXCore::cleanupDevice();
}

inline HRESULT D3D_RestoreDevice()
{
    return GFXCore::restoreDevice();
}

inline HRESULT D3D_ResetDevice()
{
    return GFXCore::resetDevice();
}

#endif // GFXCORE_H
