#include "gfxcore.h"

#include "gfxprivate.h"
#include "commondef.h"
#include "debug.h"

LPDIRECT3D9             GFXCore::g_pd3d = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       GFXCore::g_pd3dDevice = NULL; // Our rendering device

D3DPRESENT_PARAMETERS   GFXCore::d3dpp;

void GFXCore::setDeviceStates()
{
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    //g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);D3DTADDRESS_CLAMP

    g_pd3dDevice->SetSamplerState(0,   D3DSAMP_MAGFILTER,   D3DTEXF_LINEAR );//D3DTEXF_LINEAR
    g_pd3dDevice->SetSamplerState(0,   D3DSAMP_MINFILTER,   D3DTEXF_LINEAR );//
    //g_pd3dDevice->SetSamplerState(0,   D3DSAMP_MIPFILTER,   D3DTEXF_LINEAR);

    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//D3DTOP_MODULATE
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}

DWORD GFXCore::createTexture(const char* filename, Texture* tex, DWORD w, DWORD h)
{
    return D3DXCreateTextureFromFileEx(g_pd3dDevice, filename, w, h, 0, 0,
                                       D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_DEFAULT,
                                       0, NULL, NULL, tex);
}

HRESULT GFXCore::initDevice(HWND hWnd)
{
    HRESULT hr;

    g_pd3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(!g_pd3d)
        return E_FAIL;

    D3DDISPLAYMODE displayMode;
    if(FAILED(g_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
        return E_FAIL;

    D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;
    if(g_pd3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, true, D3DMULTISAMPLE_4_SAMPLES, NULL) == D3D_OK)
        multiType = D3DMULTISAMPLE_4_SAMPLES;

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
    d3dpp.MultiSampleType = multiType;

    #ifdef WINDOWED
    d3dpp.Windowed = true;
    #else
    d3dpp.Windowed = false;
    #endif

    // Create the D3DDevice
    if( FAILED( hr=g_pd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_FPU_PRESERVE|D3DCREATE_MULTITHREADED,
                                      &d3dpp, &g_pd3dDevice ) ) )
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

    setDeviceStates();

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
    if(g_pd3dDevice)
        g_pd3dDevice->Release();
    g_pd3dDevice=0;

    if(g_pd3d)
        g_pd3d->Release();
    g_pd3d=0;

}

HRESULT GFXCore::restoreDevice()
{
    HRESULT hr;
    while((hr=g_pd3dDevice->TestCooperativeLevel())!=D3DERR_DEVICENOTRESET)
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
    if(FAILED(hr=g_pd3dDevice->Reset(&d3dpp)))
        return hr;
    setDeviceStates();
    return S_OK;
}
