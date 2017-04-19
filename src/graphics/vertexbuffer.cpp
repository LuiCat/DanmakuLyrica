#include "vertexbuffer.h"

#include "commondef.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define D3D_VERTEXBUFFERSIZE 300000
#define D3D_VERTEXCHUNKSIZE 20000

#define INITSIZE_STACK_MATRIX 64
#define INITSIZE_PENDING_VERTEX 4096

VertexBuffer d3d;

VertexBuffer::VertexBuffer()
    :hWnd(0)
    ,pD3DVertexBuffer(0)
    ,mutex(0)
    ,currentTarget(0)
    ,currentChunkOffset(0)
    ,lastRenderTexture(0)
    ,lastRenderBlendDest(0)
{
    pendingVertices.reserve(INITSIZE_PENDING_VERTEX);
    stackMatrix.reserve(INITSIZE_STACK_MATRIX);
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

void VertexBuffer::renderResetState()
{
    lastRenderTexture = 0;
    lastRenderBlendDest = 0;
}

void VertexBuffer::renderSetTexture(Texture texture)
{
    if (lastRenderTexture == texture)
        return;
    pD3DDevice->SetTexture(0, texture);
    lastRenderTexture = texture;
}

void VertexBuffer::renderSetBlendDest(DWORD blend)
{
    if (lastRenderBlendDest == blend)
        return;
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, blend);
    lastRenderBlendDest = blend;
}

int VertexBuffer::primitiveCount(int vertexCount, ShapeType shape)
{
    int result = 0;
    switch (shape)
    {
    case Shape_Lines: result = vertexCount - 1; break;
    case Shape_Triangles: result = vertexCount - 2; break;
    default: return 0;
    }
    return result < 0 ? 0 : result;
}

void VertexBuffer::drawScene(RenderTarget* target)
{
    if (FAILED(pD3DDevice->BeginScene()))
        return;

    if (target)
        target->setTarget();

    pD3DDevice->SetStreamSource(0, pD3DVertexBuffer, 0, sizeof(Vertex));
    pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
    renderResetState();

    UINT vertexSize = pendingVertices.size() / 4 * 4;
    UINT vertexCount = 0;
    UINT lastIndex = currentChunkOffset;
    UINT lastVertex = 0;
    UINT currVertex = 0;
    Vertex* pVertices;

    if (FAILED(pD3DVertexBuffer->Lock(currentChunkOffset * sizeof(Vertex), D3D_VERTEXCHUNKSIZE * sizeof(Vertex),
                                      (LPVOID*)&pVertices, currentChunkOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
        return;

    for (UINT i = 0; i < vertexSize;)
    {
        *pVertices = pendingVertices[i++].vertex;
        ++pVertices;
        ++vertexCount;

        if (vertexCount == D3D_VERTEXCHUNKSIZE || i == vertexSize)
        {
            pD3DVertexBuffer->Unlock();

            for (UINT j = 0; j < vertexCount && currVertex < vertexSize; ++j)
            {
                auto& info = pendingVertices[currVertex++];
                if (info.shape == Shape_None)
                    continue;

                renderSetBlendDest(info.isAddBlend ? D3DBLEND_ONE : D3DBLEND_INVSRCALPHA);
                renderSetTexture(info.texture);
                pD3DDevice->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(info.shape), lastIndex,
                                          primitiveCount(currVertex - lastVertex, info.shape));
                lastVertex = currVertex;
                lastIndex = currentChunkOffset + j + 1;
            }

            vertexCount = 0;
            currentChunkOffset += D3D_VERTEXCHUNKSIZE;
            if (currentChunkOffset >= D3D_VERTEXBUFFERSIZE)
                currentChunkOffset = 0;

            if (i == vertexSize)
                break;

            if (FAILED(pD3DVertexBuffer->Lock(currentChunkOffset * sizeof(Vertex), D3D_VERTEXCHUNKSIZE * sizeof(Vertex),
                (LPVOID*)&pVertices, currentChunkOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD)))
                return;

            if (currentChunkOffset == 0)
            {
                currVertex = lastVertex;
                i = lastVertex;
                lastIndex = 0;
            }

        }

    }

    /*
    for (int j = 0; j < vertexCount && currVertex < vertexSize; ++j)
    {
        auto& info = pendingVertices[currVertex++];
        if (info.shape == Shape_None)
            continue;
        renderSetBlendDest(info.isAddBlend ? D3DBLEND_ONE : D3DBLEND_INVSRCALPHA);
        renderSetTexture(info.texture);
        pD3DDevice->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(info.shape), lastIndex, 2);
        //primitiveCount(currVertex - lastVertex + 1, info.shape));
        lastVertex = currVertex;
        lastIndex = currentChunkOffset + j + 1;
    }*/

    pendingVertices.clear();

    if (target)
        target->unsetTarget();

    pD3DDevice->EndScene();

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
    //pendingVertices.clear();
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

void VertexBuffer::rect(float u1, float v1, float u2, float v2)
{
    d3d.vertex(0.0, 0.0, u1, v1);
    d3d.vertex(1.0, 0.0, u2, v1);
    d3d.vertex(0.0, 1.0, u1, v2);
    d3d.vertex(1.0, 1.0, u2, v2);
    d3d.shape(Shape_Triangles);
}

void VertexBuffer::rect(float x, float y, float w, float h, float u1, float v1, float u2, float v2)
{
    float x2 = x + w, y2 = y + h;
    d3d.vertex(x,  y,  u1, v1);
    d3d.vertex(x2, y,  u2, v1);
    d3d.vertex(x,  y2, u1, v2);
    d3d.vertex(x2, y2, u2, v2);
    d3d.shape(Shape_Triangles);
}

void VertexBuffer::vertex(float x, float y, float u, float v)
{
    pendingVertices.push_back(VertexInfo{x, y, 0.0f, 1.0f, currentMatrix.defaultColor,
                                         u, v, currentMatrix.texture, currentMatrix.isAddBlend, Shape_None});
    VertexInfo& tmp = pendingVertices.back();
    D3DXVec3TransformCoord((Vector*)&tmp.vertex, (Vector*)&tmp.vertex, &currentMatrix.matrix);
}

void VertexBuffer::vertex(float x, float y, float z, float u, float v)
{
    pendingVertices.push_back(VertexInfo{x, y, z, 1.0f, currentMatrix.defaultColor,
                                         u, v, currentMatrix.texture, currentMatrix.isAddBlend, Shape_None });
    VertexInfo& tmp = pendingVertices.back();
    D3DXVec3TransformCoord((Vector*)&tmp.vertex, (Vector*)&tmp.vertex, &currentMatrix.matrix);
}

void VertexBuffer::shape(ShapeType shape)
{
    if (!pendingVertices.empty())
        pendingVertices.back().shape = shape;
}

//-----------------------------------------------------------------------------------------------------------

void VertexBuffer::translate2D(float x, float y)
{
    Matrix t;
    D3DXMatrixTranslation(&t, x, y, 0.0);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::rotate2D(float angle)
{
    Matrix t;
    D3DXMatrixRotationZ(&t, angle);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::scale2D(float scaleX, float scaleY)
{
    Matrix t;
    D3DXMatrixScaling(&t, scaleX, scaleY, 1.0);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::translate3D(float x, float y, float z)
{
    Matrix t;
    D3DXMatrixTranslation(&t, x, y, z);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::rotate3D(float axisX, float axisY, float axisZ, float angle)
{
    Matrix t;
    D3DXVECTOR3 v(axisX, axisY, axisZ);
    D3DXMatrixRotationAxis(&t, &v, angle);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void VertexBuffer::scale3D(float scaleX, float scaleY, float scaleZ)
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
