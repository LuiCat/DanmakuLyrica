#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "gfxresource.h"
#include "gfxdef.h"
#include "rendertarget.h"

#include <vector>
using namespace std;

#define D3D_VERTEXBUFFERSIZE 120000

#define INITSIZE_STACK_MATRIX 64
#define INITSIZE_PENDING_VERTEX 1024

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
    };

    struct MatrixInfo
    {
        Matrix matrix;
        Texture texture;
        DWORD defaultColor;
        bool isAddBlend;
    };

    vector<VertexInfo> pendingVertex;
    vector<MatrixInfo> stackMatrix;

    MatrixInfo currentMatrix;

    LPDIRECT3DVERTEXBUFFER9 pD3DVertexBuffer;

    HANDLE mutex;

    RenderTarget* currentTarget;

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

    void vertex(double x, double y, double u, double v);
    void vertex(double x, double y, double z, double u, double v);

    void translate2D(double x, double y);
    void rotate2D(double angle);
    void scale2D(double scaleX, double scaleY);

    void translate3D(double x, double y, double z);
    void rotate3D(double axisX, double axisY, double axisZ, double angle);
    void scale3D(double scaleX, double scaleY, double scaleZ);

    void setBlend(bool add);
    void setTexture(Texture tex);
    void setAlpha(double alpha);
    void setColor(DWORD color);

};

extern VertexBuffer d3d;

#endif // VERTEXBUFFER_H
