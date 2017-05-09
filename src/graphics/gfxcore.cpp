#include "gfxcore.h"

#include "commondef.h"
#include "debug.h"

HWND                    GFXCore::hWnd = NULL;

LPDIRECT3D9             GFXCore::pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       GFXCore::pD3DDevice = NULL; // Our rendering device

D3DPRESENT_PARAMETERS   GFXCore::d3dpp;

void GFXCore::initDeviceStates()
{
    pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    //g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);D3DTADDRESS_CLAMP

    pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    pD3DDevice->SetSamplerState(0,   D3DSAMP_MAGFILTER,   D3DTEXF_LINEAR );//D3DTEXF_LINEAR
    pD3DDevice->SetSamplerState(0,   D3DSAMP_MINFILTER,   D3DTEXF_LINEAR );//
    //pD3DDevice->SetSamplerState(0,   D3DSAMP_MIPFILTER,   D3DTEXF_LINEAR);

    pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
    pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);

    pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//D3DTOP_MODULATE
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}

DWORD GFXCore::createTexture(const char* filename, Texture* tex, DWORD w, DWORD h)
{
    return D3DXCreateTextureFromFileEx(pD3DDevice, filename, w, h, 0, 0,
                                       D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                       0, NULL, NULL, tex);
}

HRESULT GFXCore::initDevice(HWND hWnd)
{
    HRESULT hr;

    GFXCore::hWnd = hWnd;

    pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(!pD3D)
        return E_FAIL;

    D3DDISPLAYMODE displayMode;
    if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
        return E_FAIL;

    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

    d3dpp.hDeviceWindow = hWnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
#ifndef FPS_VSYNC
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#else
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
#endif
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = WIDTH;
    d3dpp.BackBufferHeight = HEIGHT;
#ifdef WINDOWED
    d3dpp.Windowed = true;
#else
    d3dpp.Windowed = false;
#endif

    D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;
    DWORD quality;
    if(pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, d3dpp.Windowed, D3DMULTISAMPLE_4_SAMPLES, &quality) == D3D_OK)
        multiType = D3DMULTISAMPLE_4_SAMPLES;
    d3dpp.MultiSampleType = multiType;
    d3dpp.MultiSampleQuality = (quality>0?quality-1:0);


    // Create the D3DDevice
    if( FAILED( hr=pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_FPU_PRESERVE|D3DCREATE_MULTITHREADED,
                                      &d3dpp, &pD3DDevice ) ) )
    {
        switch(hr){
        case D3DERR_DEVICELOST:
            cout<<"D3DERR_DEVICELOST"<<endl;
            break;
        case D3DERR_INVALIDCALL:
            cout<<"D3DERR_INVALIDCALL"<<endl;
            break;
        case D3DERR_NOTAVAILABLE:
            cout<<"D3DERR_NOTAVAILABLE"<<endl;
            break;
        case D3DERR_OUTOFVIDEOMEMORY:
            cout<<"D3DERR_OUTOFVIDEOMEMORY"<<endl;
            break;
        }
        return hr;
    }

    initDeviceStates();

    /*
    if(FAILED(hr=g_pd3dDevice->CreateRenderTarget(WIDTH, HEIGHT, d3dpp.BackBufferFormat,
                                     d3dpp.MultiSampleType, d3dpp.MultiSampleQuality, FALSE,
                                     &g_pd3dSurface, NULL)))
    {
        cout<<"failed to create render target"<<endl;
    }
    */

    return S_OK;
}

void GFXCore::cleanupDevice()
{
    if(pD3DDevice)
        pD3DDevice->Release();
    pD3DDevice=0;

    if(pD3D)
        pD3D->Release();
    pD3D=0;

}

HRESULT GFXCore::restoreDevice()
{
    HRESULT hr;
    while((hr=pD3DDevice->TestCooperativeLevel())!=D3DERR_DEVICENOTRESET)
    {
        if(hr==D3DERR_DRIVERINTERNALERROR)
            return hr;
        Sleep(30);
    }
    return S_OK;
}

HRESULT GFXCore::resetDevice()
{
    HRESULT hr;
    if(FAILED(hr=pD3DDevice->Reset(&d3dpp)))
        return hr;
    initDeviceStates();
    return S_OK;
}

void GFXCore::present()
{
    HRESULT hr;

    hr = pD3DDevice->Present(NULL, NULL, NULL, NULL);

    if (FAILED(hr))
    {
        if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
            SendMessage(hWnd, WM_RESETDEVICE, 0, 0);
        else
            PostQuitMessage(hr);
    }

    pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

}
