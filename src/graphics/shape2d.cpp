#include "shape2d.h"

#include "commondef.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define D3D_VERTEXBUFFERSIZE 300000
#define D3D_VERTEXCHUNKSIZE 20000

#define INITSIZE_STACK_MATRIX 64
#define INITSIZE_PENDING_VERTEX 4096
#define INITSIZE_LIST_SHAPE 1024

using namespace std;

// Global Shape2d instance
Shape2D d2d;

Shape2D::Shape2D()
    :pD3DVertexBuffer(0)
    ,currentChunkOffset(0)
    ,lastRenderTexture(0)
    ,lastRenderBlendDest(0)
{
    pendingVertices.reserve(INITSIZE_PENDING_VERTEX);
    stackMatrix.reserve(INITSIZE_STACK_MATRIX);
    listShapes.reserve(INITSIZE_LIST_SHAPE);
}

Shape2D::~Shape2D()
{

}

HRESULT Shape2D::init()
{
    HRESULT hr;

    if(FAILED(hr=pD3DDevice->CreateVertexBuffer(D3D_VERTEXBUFFERSIZE*sizeof(Vertex),
                                                  D3DUSAGE_DYNAMIC, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &pD3DVertexBuffer, NULL)))
    {
        return hr;
    }

    resetMatrix();

    return S_OK;
}

void Shape2D::cleanup()
{
    if(pD3DVertexBuffer!=0)
        pD3DVertexBuffer->Release();
    pD3DVertexBuffer=0;
}

void Shape2D::renderResetState()
{
    lastRenderTexture = 0;
    lastRenderBlendDest = D3DBLEND_INVSRCALPHA;
    pD3DDevice->SetTexture(0, 0);
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Shape2D::renderSetTexture(Texture texture)
{
    if (lastRenderTexture == texture)
        return;
    pD3DDevice->SetTexture(0, texture);
    lastRenderTexture = texture;
}

void Shape2D::renderSetBlendDest(DWORD blend)
{
    if (lastRenderBlendDest == blend)
        return;
    pD3DDevice->SetRenderState(D3DRS_DESTBLEND, blend);
    lastRenderBlendDest = blend;
}

void Shape2D::clearVertices()
{
    pendingVertices.resize(0);
    listShapes.resize(0);
}

int Shape2D::primitiveCount(int vertexCount, ShapeType shape)
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

void Shape2D::drawScene(RenderTarget* target)
{
    if (FAILED(pD3DDevice->BeginScene()))
        return;

    if (target)
        target->setTarget();

    pD3DDevice->SetStreamSource(0, pD3DVertexBuffer, 0, sizeof(Vertex));
    pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
    renderResetState();

    UINT vertexSize = pendingVertices.size();
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
                if (info.shape == -1)
                    continue;

                renderSetBlendDest(info.isAddBlend ? D3DBLEND_ONE : D3DBLEND_INVSRCALPHA);
                renderSetTexture(info.texture);
                pD3DDevice->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(listShapes[info.shape].shape), lastIndex,
                                          primitiveCount(currVertex - lastVertex, listShapes[info.shape].shape));
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

    clearVertices();

    if (target)
        target->unsetTarget();

    pD3DDevice->EndScene();

}

void Shape2D::beginScene()
{
    mutex.lock();
}

void Shape2D::endScene()
{
    mutex.unlock();
}

void Shape2D::resetMatrix()
{
    //pendingVertices.clear();
    stackMatrix.clear();
    identityMatrix();
    currentMatrix.texture=0;
    currentMatrix.isAddBlend=false;
    currentMatrix.defaultColor=0xffffffff;
}

void Shape2D::identityMatrix()
{
    D3DXMatrixIdentity(&currentMatrix.matrix);
    D3DXMatrixTranslation(&currentMatrix.matrix, -0.5, -0.5, 0.0);
}

void Shape2D::pushMatrix()
{
    stackMatrix.push_back(currentMatrix);
}

void Shape2D::popMatrix()
{
    if(!stackMatrix.empty())
    {
        currentMatrix=stackMatrix.back();
        stackMatrix.pop_back();
    }
}

void Shape2D::rect(float u1, float v1, float u2, float v2)
{
    d2d.vertex(0.0, 0.0, u1, v1);
    d2d.vertex(1.0, 0.0, u2, v1);
    d2d.vertex(0.0, 1.0, u1, v2);
    d2d.vertex(1.0, 1.0, u2, v2);
    d2d.shape(Shape_Triangles);
}

void Shape2D::rect(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    d2d.vertex(x1, y1, u1, v1);
    d2d.vertex(x2, y1, u2, v1);
    d2d.vertex(x1, y2, u1, v2);
    d2d.vertex(x2, y2, u2, v2);
    d2d.shape(Shape_Triangles);
}

void Shape2D::vertex(float x, float y, float u, float v)
{
    pendingVertices.push_back(VertexInfo{x, y, 0.0f, 1.0f, currentMatrix.defaultColor,
                                         u, v, currentMatrix.texture, currentMatrix.isAddBlend, -1});
    VertexInfo& tmp = pendingVertices.back();
    D3DXVec3TransformCoord((Vector*)&tmp.vertex, (Vector*)&tmp.vertex, &currentMatrix.matrix);
}

void Shape2D::shape(ShapeType shape)
{
    if (!pendingVertices.empty())
    {
		pendingVertices.back().shape = listShapes.size();
        listShapes.push_back(ShapeInfo{ shape, currentMatrix.matrix });
    }
}

//-----------------------------------------------------------------------------------------------------------

void Shape2D::translate(float x, float y)
{
    Matrix t;
    D3DXMatrixTranslation(&t, x, y, 0.0);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void Shape2D::rotate(float angle)
{
    Matrix t;
    D3DXMatrixRotationZ(&t, angle);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void Shape2D::scale(float scaleX, float scaleY)
{
    Matrix t;
    D3DXMatrixScaling(&t, scaleX, scaleY, 1.0);
    currentMatrix.matrix=t*currentMatrix.matrix;
}

void Shape2D::setBlend(bool add)
{
    currentMatrix.isAddBlend=add;
}

void Shape2D::setTexture(Texture tex)
{
    currentMatrix.texture=tex;
}

void Shape2D::setAlpha(double alpha)
{
    if(alpha<0.0)alpha=0.0;
    else if(alpha>1.0)alpha=1.0;
    alpha*=0xff000000;
    currentMatrix.defaultColor&=0xffffff;
    currentMatrix.defaultColor|=(((unsigned long)alpha)&0xff000000);
}

void Shape2D::setColor(DWORD color)
{
    currentMatrix.defaultColor&=0xff000000;
    currentMatrix.defaultColor|=(color&0xffffff);
}
