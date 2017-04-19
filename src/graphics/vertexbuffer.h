#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "gfxresource.h"
#include "gfxdef.h"
#include "rendertarget.h"

#include <vector>
using namespace std;

enum ShapeType
{
    Shape_None = 0,
    Shape_Lines = D3DPT_LINELIST,
    Shape_Triangles = D3DPT_TRIANGLESTRIP
};

class VertexBuffer : public GFXCore
{
private:

    HWND hWnd;

    struct Vertex
    {
        float x, y, z, rhw;
        DWORD color;
        float u, v;
    };

    struct VertexInfo
    {
        Vertex vertex;
        Texture texture;
        bool isAddBlend;
        ShapeType shape;
    };

    struct MatrixInfo
    {
        Matrix matrix;
        Texture texture;
        DWORD defaultColor;
        bool isAddBlend;
    };

    vector<VertexInfo> pendingVertices;
    vector<MatrixInfo> stackMatrix;

    MatrixInfo currentMatrix;

    LPDIRECT3DVERTEXBUFFER9 pD3DVertexBuffer;

    HANDLE mutex;

    RenderTarget* currentTarget;

    UINT currentChunkOffset;

    Texture lastRenderTexture;
    DWORD lastRenderBlendDest;

    void renderResetState();
    void renderSetTexture(Texture texture);
    void renderSetBlendDest(DWORD blend);

    int primitiveCount(int vertexCount, ShapeType shape);

protected:

    void drawScene(RenderTarget* target=0);

public:

    VertexBuffer();
    ~VertexBuffer();

    HRESULT init(HWND hWnd=0);
    void cleanup();

    void present();

    void beginScene(RenderTarget* target=0);
    void endScene();

    void resetMatrix();

    void pushMatrix();
    void popMatrix();

    void rect(float u1, float v1, float u2, float v2);
    void rect(float x, float y, float w, float h, float u1, float v1, float u2, float v2);

    void vertex(float x, float y, float u, float v);
    void vertex(float x, float y, float z, float u, float v);
    // make a shape depend on vertices pushed before, 
    void shape(ShapeType shape = Shape_Triangles);

    void translate2D(float x, float y);
    void rotate2D(float angle);
    void scale2D(float scaleX, float scaleY);

    void translate3D(float x, float y, float z);
    void rotate3D(float axisX, float axisY, float axisZ, float angle);
    void scale3D(float scaleX, float scaleY, float scaleZ);

    void setBlend(bool add);
    void setTexture(Texture tex);
    void setAlpha(double alpha);
    void setColor(DWORD color);

};

extern VertexBuffer d3d;

#endif // VERTEXBUFFER_H
