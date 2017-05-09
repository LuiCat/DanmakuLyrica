#ifndef _SHAPE2D_H_
#define _SHAPE2D_H_

#include "gfxresource.h"
#include "gfxdef.h"
#include "rendertarget.h"

#include <vector>
#include <mutex>

class Shape2D : public GFXCore
{
private:

    struct Vertex
    {
        float x, y, z, rhw;
        DWORD color;
        float u, v;
    };

    struct ShapeInfo
    {
        ShapeType shape;
        Matrix matrix;
    };

    struct VertexInfo
    {
        Vertex vertex;
        Texture texture;
        bool isAddBlend;
        ShapeInfo* shape;
    };

    struct MatrixInfo
    {
        Matrix matrix;
        Texture texture;
        DWORD defaultColor;
        bool isAddBlend;
    };

    std::vector<VertexInfo> pendingVertices;
    std::vector<MatrixInfo> stackMatrix;
    std::vector<ShapeInfo> listShapes;

    MatrixInfo currentMatrix;

    LPDIRECT3DVERTEXBUFFER9 pD3DVertexBuffer;

    std::mutex mutex;

    UINT currentChunkOffset;

    Texture lastRenderTexture;
    DWORD lastRenderBlendDest;

    void renderResetState();
    void renderSetTexture(Texture texture);
    void renderSetBlendDest(DWORD blend);

    void clearVertices();

    int primitiveCount(int vertexCount, ShapeType shape);

public:

    Shape2D();
    ~Shape2D();

    HRESULT init();
    void cleanup();

    void beginScene();
    void endScene();

    void drawScene(RenderTarget* target = 0);

    void resetMatrix();
    void identityMatrix();

    void pushMatrix();
    void popMatrix();

    void rect(float u1, float v1, float u2, float v2);
    void rect(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2);

    void vertex(float x, float y, float u, float v);
    // make a shape depend on vertices pushed before, 
    void shape(ShapeType shape = Shape_Triangles);

    void translate(float x, float y);
    void rotate(float angle);
    void scale(float scaleX, float scaleY);

    void setBlend(bool add);
    void setTexture(Texture tex);
    void setAlpha(double alpha);
    void setColor(DWORD color);

};

extern Shape2D d2d;

#endif // _SHAPE2D_H_
