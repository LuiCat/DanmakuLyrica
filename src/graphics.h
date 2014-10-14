#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
using namespace std;

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

typedef  D3DXMATRIX          Matrix  ;
typedef  LPDIRECT3DTEXTURE9  Texture ;
typedef  D3DXVECTOR3         Vector  ;

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

class VertexBuffer
{
private:

    HWND hWnd;

    vector<VertexInfo> pendingVertex;
    vector<MatrixInfo> stackMatrix;

    MatrixInfo currentMatrix;

	LPDIRECT3DVERTEXBUFFER9 pD3DVertexBuffer;

    HANDLE mutex;

public:

    VertexBuffer();
    ~VertexBuffer();

    HRESULT init(HWND hWnd);
    HRESULT reset();
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

DWORD createTexture(const char *filename, Texture* tex, DWORD w=D3DX_DEFAULT, DWORD h=D3DX_DEFAULT);

HRESULT D3D_Init(HWND hWnd);
void D3D_Cleanup();

extern VertexBuffer d3d;

#endif
