/*!
  \file graphics.h
  \brief Contains important functions for graphics initialization
  */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "gfxdef.h"
#include "gfxcore.h"
#include "shape2d.h"
#include "shape3d.h"
#include "rendersurface.h"
#include "rendertexture.h"
#include "textrenderer.h"

inline HRESULT D3D_Init(HWND hWnd)
{
    if(FAILED(D3D_InitDevice(hWnd)))
        return E_FAIL;
    if(FAILED(d2d.init()))
        return E_FAIL;
    if(FAILED(d3d.init()))
        return E_FAIL;
    return S_OK;
}

inline void D3D_Cleanup()
{
    d3d.cleanup();
    d2d.cleanup();
    D3D_CleanupDevice();
}

inline HRESULT D3D_Reset()
{
    try
    {
        d2d.beginScene();
        d3d.beginScene();

        if(FAILED(D3D_RestoreDevice()))
            throw 0;

        GFXResource::lostAll();

        d3d.cleanup();
        d2d.cleanup();

        if(FAILED(D3D_ResetDevice()))
            throw 1;

        if(FAILED(d2d.init()))
            throw 2;
        if(FAILED(d3d.init()))
            throw 2;

        GFXResource::resetAll();

        d3d.endScene();
        d2d.endScene();

        return S_OK;
    }
    catch(int)
    {

    }

    d2d.endScene();

    return E_FAIL;

}

#endif
