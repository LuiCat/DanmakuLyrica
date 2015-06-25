#include "vertexbuffer.h"

#include "commondef.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

VertexBuffer d3d;

VertexBuffer::VertexBuffer()
    :hWnd(0)
    ,pendingVertex(INITSIZE_PENDING_VERTEX)
    ,stackMatrix(INITSIZE_STACK_MATRIX)
    ,pD3DVertexBuffer(0)
    ,mutex(0)
    ,currentTarget(0)
{

}

VertexBuffer::~VertexBuffer()
{

}

HRESULT VertexBuffer::init(HWND hWnd)
{
    HRESULT hr;

    if(hWnd)
        this->hWnd=hWnd;

    if(FAILED(hr=pD3DDevice->CreateVertexBuffer(D3D_VERTEXBUFFERSIZE*sizeof(Vertex),
                                                  D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &pD3DVertexBuffer, NULL)))
    {
        return hr;
    }

    resetMatrix();

    if(!mutex)
        mutex=CreateMutex(0, false, "D3DMutex");

    return S_OK;
}

void VertexBuffer::cleanup()
{
    if(pD3DVertexBuffer!=0)
        pD3DVertexBuffer->Release();
    pD3DVertexBuffer=0;
}

void VertexBuffer::drawScene(RenderTarget* target)
{
    int vsize=pendingVertex.size()/4*4;
    Vertex* pVertices;

    if(FAILED(pD3DVertexBuffer->Lock(0, vsize*sizeof(Vertex), (LPVOID*)&pVertices, 0)))
        return;

    for(int i=0;i<vsize;++i)
    {
        pVertices[i]=pendingVertex[i].vertex;
    }

    pD3DVertexBuffer->Unlock();

    if(SUCCEEDED(pD3DDevice->BeginScene()))
    {
        if(target)
            target->setTarget();

        pD3DDevice->SetStreamSource(0, pD3DVertexBuffer, 0, sizeof(Vertex));
        pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

        for(unsigned int i=0;i<pendingVertex.size();i+=4)
        {
            if(pendingVertex[i].isAddBlend)
                pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            else
                pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            pD3DDevice->SetTexture(0, pendingVertex[i].texture);
            pD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, i, 2);
        }

        if(target)
            target->unsetTarget();

        //g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &Backbuffer);
        //hr=g_pd3dDevice->StretchRect(g_pd3dSurface, 0, Backbuffer, 0, D3DTEXF_NONE);

        pD3DDevice->EndScene();
    }

}

void VertexBuffer::present()
{
    HRESULT hr;

    hr=pD3DDevice->Present( NULL, NULL, NULL, NULL );

    if(FAILED(hr))
    {
        if(hr==D3DERR_DEVICELOST||hr==D3DERR_DEVICENOTRESET)
            SendMessage(hWnd, WM_RESETDEVICE, 0, 0);
        else
            PostQuitMessage(hr);
    }

    pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

}

void VertexBuffer::beginScene(RenderTarget* target)
{
    WaitForSingleObject(mutex, INFINITE);
    currentTarget=target;
    resetMatrix();
}

void VertexBuffer::endScene()
{
    drawScene(currentTarget);
    currentTarget=0;
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

void VertexBuffer::vertex(double x, double y, double u, double v)
{
    VertexInfo tmp={(float)x, (float)y, 0.0f, 1.0f, currentMatrix.defaultColor, (float)u, (float)v, currentMatrix.texture, currentMatrix.isAddBlend};
    D3DXVec3TransformCoord((Vector*)&tmp.vertex, (Vector*)&tmp.vertex, &currentMatrix.matrix);
    pendingVertex.push_back(tmp);
}

void VertexBuffer::vertex(double x, double y, double z, double u, double v)
{
    VertexInfo tmp={(float)x, (float)y, (float)z, 1.0f, currentMatrix.defaultColor, (float)u, (float)v, currentMatrix.texture, currentMatrix.isAddBlend};
    D3DXVec3TransformCoord((Vector*)&tmp.vertex, (Vector*)&tmp.vertex, &currentMatrix.matrix);
    pendingVertex.push_back(tmp);
}

//-----------------------------------------------------------------------------------------------------------

void VertexBuffer::translate2D(double x, double y)
{
    Matrix t;
    D3DXMatrixTranslation(&t, x, y, 0.0);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::rotate2D(double angle)
{
    Matrix t;
    D3DXMatrixRotationZ(&t, angle);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::scale2D(double scaleX, double scaleY)
{
    Matrix t;
    D3DXMatrixScaling(&t, scaleX, scaleY, 1.0);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::translate3D(double x, double y, double z)
{
    Matrix t;
    D3DXMatrixTranslation(&t, x, y, z);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::rotate3D(double axisX, double axisY, double axisZ, double angle)
{
    Matrix t;
    D3DXVECTOR3 v(axisX, axisY, axisZ);
    D3DXMatrixRotationAxis(&t, &v, angle);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::scale3D(double scaleX, double scaleY, double scaleZ)
{
    Matrix t;
    D3DXMatrixScaling(&t, scaleX, scaleY, scaleZ);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::setBlend(bool add)
{
    currentMatrix.isAddBlend=add;
}

void VertexBuffer::setTexture(Texture tex)
{
    currentMatrix.texture=tex;
}

void VertexBuffer::setAlpha(double alpha)
{
    if(alpha<0.0)alpha=0.0;
    else if(alpha>1.0)alpha=1.0;
    alpha*=0xff000000;
    currentMatrix.defaultColor&=0xffffff;
    currentMatrix.defaultColor|=(((unsigned long)alpha)&0xff000000);
}

void VertexBuffer::setColor(DWORD color)
{
    currentMatrix.defaultColor&=0xff000000;
    currentMatrix.defaultColor|=(color&0xffffff);
}
