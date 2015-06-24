#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "gfxcore.h"
#include "gfxdef.h"

#include <vector>
using namespace std;

#define D3D_VERTEXBUFFERSIZE 120000

class VertexBuffer
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

protected:

    HRESULT onLost();
    HRESULT onReset();

public:

    VertexBuffer();
    ~VertexBuffer();

    HRESULT init(HWND hWnd=0);
    void cleanup();

    void drawMatrix();
    void present();

    void beginScene();
    void endScene();

    void resetMatrix();

    void pushMatrix();
    void popMatrix();

    void pushVertex(double x, double y, double u, double v);

    void translate2D(double x, double y);
    void rotate2D(double rotation);
    void scale2D(double scaleX, double scaleY);

    void setBlend(bool add);
    void setTexture(Texture tex);
    void setAlpha(double alpha);
    void setColor(DWORD color);

};

extern VertexBuffer d3d;

#endif // VERTEXBUFFER_H
