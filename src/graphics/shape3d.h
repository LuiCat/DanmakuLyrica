// 编码：Utf-8 without BOM
// Created by LuiCat

#ifndef _SHAPE3D_H_
#define _SHAPE3D_H_

#include "gfxresource.h"
#include "gfxdef.h"
#include "rendertarget.h"

#include <vector>
#include <mutex>

class Shape3D : public GFXCore
{
private:

    struct Vertex
    {
        float x, y, z;
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

    Shape3D();
    ~Shape3D();

    HRESULT init();
    void cleanup();

    void beginScene();
    void endScene();

    void drawScene(RenderTarget* target = 0);

    void resetMatrix();
    void identityMatrix();

    void resetView();

    void pushMatrix();
    void popMatrix();

    // some shapes for testing
    //void sphere(...);

    void vertex(float x, float y, float z, float u = 0, float v = 0);
    // make a shape depend on vertices pushed before, 
    void shape(ShapeType shape = Shape_Triangles);

    void translate(float x, float y, float z);
    void rotate(float axisX, float axisY, float axisZ, float angle);
    void scale(float scaleX, float scaleY, float scaleZ);
    
    // todo: a better blend function? (without shaders)
    void setBlend(bool add);
    void setTexture(Texture tex);
    void setAlpha(double alpha);
    void setColor(DWORD color);

};

extern Shape3D d3d;

#endif // _SHAPE3D_H_
