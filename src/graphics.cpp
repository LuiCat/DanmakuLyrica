#include "graphics.h"
#include "commondef.h"
#include "debug.h"

//-----------------------------------------------------------------------------

#define D3D_VERTEXBUFFERSIZE 120000

LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device

D3DPRESENT_PARAMETERS   d3dpp;

VertexBuffer d3d;

//-----------------------------------------------------------------------------

DWORD ColorBlend(DWORD col1, DWORD col2, double weight)
{
    DWORD res=0,tmp;
    for(int i=0;i<32;i+=8)
    {
        tmp=(255<<i);
        res|=(((DWORD)((col1&tmp)*weight+(col2&tmp)*(1-weight)))&tmp);
    }
    return res;
}

DWORD createTexture(const char* filename, Texture *tex, DWORD w, DWORD h)
{
    return D3DXCreateTextureFromFileEx(g_pd3dDevice, filename, w, h, 0, 0,
                                       D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_LINEAR,  D3DX_DEFAULT,
                                       0, NULL, NULL, tex);
}

//-----------------------------------------------------------------------------
//----------------class VertexBuffer functions---------------------------------
//-----------------------------------------------------------------------------

VertexBuffer::VertexBuffer()
{
    pD3DVertexBuffer=0;
}

VertexBuffer::~VertexBuffer()
{
    if(pD3DVertexBuffer!=0)
        pD3DVertexBuffer->Release();
    pD3DVertexBuffer=0;
}

HRESULT VertexBuffer::initVB()
{
    HRESULT hr;

    if(FAILED(hr=g_pd3dDevice->CreateVertexBuffer(D3D_VERTEXBUFFERSIZE*sizeof(Vertex),
                                                  D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &pD3DVertexBuffer, NULL)))
    {
        return hr;
    }

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

    return S_OK;
}

HRESULT VertexBuffer::init(HWND hWnd)
{
    this->hWnd=hWnd;

    if(FAILED(initVB()))
        return E_FAIL;

    resetMatrix();

    mutex=CreateMutex(0, false, "D3DMutex");

    return S_OK;
}

HRESULT VertexBuffer::reset()
{
    HRESULT hr;
    while((hr=g_pd3dDevice->TestCooperativeLevel())!=D3DERR_DEVICENOTRESET)
    {
        if(hr==D3DERR_DRIVERINTERNALERROR)
            return E_FAIL;
        Sleep(30);
    }
    cleanup();
    if(FAILED(hr=g_pd3dDevice->Reset(&d3dpp)))
        return hr;
    if(FAILED(hr=initVB()))
        return hr;
    return S_OK;
}

void VertexBuffer::cleanup()
{
    if(pD3DVertexBuffer!=0)
        pD3DVertexBuffer->Release();
    pD3DVertexBuffer=0;
}

void VertexBuffer::present()
{
    beginScene();

    void* pVertices;

    if(FAILED(pD3DVertexBuffer->Lock(0, (UINT)pendingVertex.size()*sizeof(Vertex), ( void** )&pVertices, 0)))
        return;

    //cout<<pendingVertex.size()<<endl;
    for(unsigned int i=0;i<pendingVertex.size();++i)
    {
        ((Vertex*)pVertices)[i]=pendingVertex[i].vertex;
    }

    pD3DVertexBuffer->Unlock();

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

    if(SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        g_pd3dDevice->SetStreamSource(0, pD3DVertexBuffer, 0, sizeof(Vertex));
        g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

        for(unsigned int i=0;i<pendingVertex.size();i+=4)
        {
            if(pendingVertex[i].isAddBlend)
                g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            else
                g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            g_pd3dDevice->SetTexture(0, pendingVertex[i].texture);

            g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i, 2);
        }
        g_pd3dDevice->EndScene();
    }

    endScene();

    HRESULT hr=g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

    if(FAILED(hr))
    {
        if(hr==D3DERR_DEVICELOST||hr==D3DERR_DEVICENOTRESET)
            SendMessage(hWnd, WM_RESETDEVICE, 0, 0);
        else
            PostQuitMessage(hr);
    }

}

void VertexBuffer::beginScene()
{
    WaitForSingleObject(mutex, INFINITE);
}

void VertexBuffer::endScene()
{
    ReleaseMutex(mutex);
}

void VertexBuffer::resetMatrix()
{
    pendingVertex.clear();
    stackMatrix.clear();

    D3DXMatrixIdentity(&currentMatrix.matrix);
    D3DXMatrixTranslation(&currentMatrix.matrix, -0.5, -0.5, 0.0);
    currentMatrix.texture=0;
    currentMatrix.isAddBlend=false;
    currentMatrix.defaultColor=0xffffffff;
}

void VertexBuffer::pushMatrix()
{
    stackMatrix.push_back(currentMatrix);
}

void VertexBuffer::popMatrix()
{
    if(!stackMatrix.empty())
    {
        currentMatrix=stackMatrix.back();
        stackMatrix.pop_back();
    }
}

void VertexBuffer::pushVertex(double x, double y, double u, double v)
{
    VertexInfo tmp={(float)x, (float)y, 0.0f, 1.0f, currentMatrix.defaultColor, (float)u, (float)v, currentMatrix.texture, currentMatrix.isAddBlend};
    D3DXVec3TransformCoord((Vector*)&tmp.vertex, (Vector*)&tmp.vertex, &currentMatrix.matrix);
    pendingVertex.push_back(tmp);
}

//-----------------------------------------------------------------------------------------------------------

HRESULT D3D_Init(HWND hWnd)
{
    HRESULT res;

    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(!g_pD3D)
        return E_FAIL;

	D3DDISPLAYMODE displayMode;
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return E_FAIL;

	D3DMULTISAMPLE_TYPE multiType = D3DMULTISAMPLE_NONE;
    if(g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, displayMode.Format, true, D3DMULTISAMPLE_4_SAMPLES, NULL) == D3D_OK)
        multiType = D3DMULTISAMPLE_4_SAMPLES;

    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
#ifdef FPS_UNLIMITED
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
	if( FAILED( res=g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING|D3DCREATE_FPU_PRESERVE|D3DCREATE_MULTITHREADED,
                                      &d3dpp, &g_pd3dDevice ) ) )
	{
		switch(res){
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
        return res;
    }

    if(FAILED(d3d.init(hWnd)))
		return E_FAIL;

    return S_OK;
}

void D3D_Cleanup()
{
    d3d.cleanup();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}
