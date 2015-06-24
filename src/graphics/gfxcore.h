#ifndef GFXCORE_H
#define GFXCORE_H

#include "gfxdef.h"

DWORD createTexture(const char *filename, Texture* tex, DWORD w=D3DX_DEFAULT, DWORD h=D3DX_DEFAULT);

HRESULT D3D_InitDevice(HWND hWnd);
void D3D_CleanupDevice();

HRESULT D3D_RestoreDevice();
HRESULT D3D_ResetDevice();

#endif // GFXCORE_H
