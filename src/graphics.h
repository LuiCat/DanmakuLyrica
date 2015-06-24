/*!
  \file graphics.h
  \brief Contains important functions for graphics initialization
  */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics/gfxdef.h"
#include "graphics/gfxcore.h"
#include "graphics/vertexbuffer.h"
#include "graphics/textrenderer.h"

inline HRESULT D3D_Init(HWND hWnd)
{
    if(FAILED(D3D_InitDevice(hWnd)))
        return E_FAIL;
    if(FAILED(d3d.init(hWnd)))
        return E_FAIL;
    return S_OK;
}

inline void D3D_Cleanup()
{
    d3d.cleanup();
    D3D_CleanupDevice();
}

inline HRESULT D3D_Reset()
{
    try
    {
        d3d.beginScene();

        if(FAILED(D3D_RestoreDevice()))
            throw 0;

        D3DResource::lostAll();

        d3d.cleanup();

        if(FAILED(D3D_ResetDevice()))
            throw 1;

        if(FAILED(d3d.init()))
            throw 2;

        D3DResource::resetAll();

        d3d.endScene();

        return S_OK;
    }
    catch(int)
    {

    }

    d3d.endScene();

    return E_FAIL;

}

#endif
