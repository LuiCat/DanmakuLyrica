#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
using namespace std;

#include "debug.h"

#define WINDOWED

#define WIDTH  800
#define HEIGHT 600

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define WM_RESETDEVICE (WM_APP|0x00)

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

protected:

    HRESULT initVB();

public:

    VertexBuffer();
    ~VertexBuffer();

    HRESULT init(HWND hWnd);
    HRESULT reset();
    void cleanup();

    void present();

    void beginScene();
    void endScene();

    void resetMatrix();

    void pushMatrix();
    void popMatrix();
	
    void pushVertex(double x, double y, double u, double v);

    inline void translate(double x, double y)
    {
        Matrix t;
        D3DXMatrixTranslation(&t, x, y, 0.0);
        currentMatrix.matrix=t*currentMatrix.matrix;
	}

    inline void rotate(double rotation)
    {
        Matrix t;
        D3DXMatrixRotationZ(&t, rotation);
        currentMatrix.matrix=t*currentMatrix.matrix;
	}

    inline void scale(double scaleX, double scaleY)
    {
        Matrix t;
        D3DXMatrixScaling(&t, scaleX, scaleY, 1.0);
        currentMatrix.matrix=t*currentMatrix.matrix;
	}

    inline void setBlend(bool add)
    {
        currentMatrix.isAddBlend=add;
	}

    inline void setTexture(Texture tex)
    {
        currentMatrix.texture=tex;
	}

    inline void setAlpha(double alpha)
    {
        currentMatrix.defaultColor&=0xffffff;
        alpha*=0xff000000;
        currentMatrix.defaultColor|=(((int)alpha)&0xff000000);
    }

    inline void setColor(DWORD color)
    {
        currentMatrix.defaultColor&=0xff000000;
        currentMatrix.defaultColor|=(color&0xffffff);
    }

};

DWORD createTexture(const char *filename, Texture* tex, DWORD w=D3DX_DEFAULT, DWORD h=D3DX_DEFAULT);

HRESULT D3D_Init(HWND hWnd);
void D3D_Cleanup();

extern VertexBuffer d3d;

#endif
