#include "imagepiece.h"
#include "texturecache.h"
#include "vertexbuffer.h"

deque<pair<ImagePiece*, string>> ImagePiece::pendingList;
bool ImagePiece::shouldLoadLater = true;

void ImagePiece::pendingLoads()
{
    shouldLoadLater = true;
}

void ImagePiece::loadPending()
{
    shouldLoadLater = false;
    while(!pendingList.empty())
    {
        auto& pair = pendingList.front();
        pair.first->tex = TextureCache::load(pair.second.c_str());
        pendingList.pop_front();
    }
}

ImagePiece::ImagePiece(Texture texture)
    : tex(texture)
    , umin(0.0)
    , vmin(0.0)
    , umax(1.0)
    , vmax(1.0)
{

}

ImagePiece::ImagePiece(Texture texture, double minU, double minV, double maxU, double maxV)
    : tex(texture)
    , umin(minU)
    , vmin(minV)
    , umax(maxU)
    , vmax(maxV)
{

}

ImagePiece::ImagePiece(const char* filename)
    : tex(0)
    , umin(0.0)
    , vmin(0.0)
    , umax(1.0)
    , vmax(1.0)
{
    loadOrLater(filename);
}

ImagePiece::ImagePiece(const char* filename, double minU, double minV, double maxU, double maxV)
    : tex(0)
    , umin(minU)
    , vmin(minV)
    , umax(maxU)
    , vmax(maxV)
{
    loadOrLater(filename);
}

void ImagePiece::loadOrLater(const char* filename)
{
    if(!shouldLoadLater)
        tex = TextureCache::load(filename);
    pendingList.emplace_back(this, filename);
}

void ImagePiece::load(const char* filename)
{
    loadOrLater(filename);
    umin = 0.0;
    vmin = 0.0;
    umax = 1.0;
    vmax = 1.0;
}

void ImagePiece::load(const char* filename, double minU, double minV, double maxU, double maxV)
{
    loadOrLater(filename);
    umin = minU;
    vmin = minV;
    umax = maxU;
    vmax = maxV;
}

void ImagePiece::setTexture(Texture texture)
{
    tex = texture;
}

void ImagePiece::setRenderRange(double minU, double minV, double maxU, double maxV)
{
    umin = minU;
    vmin = minV;
    umax = maxU;
    vmax = maxV;
}

void ImagePiece::vertice()
{
    d3d.setTexture(tex);
    d3d.vertex(0.0, 0.0, umin, vmin);
    d3d.vertex(1.0, 0.0, umax, vmin);
    d3d.vertex(1.0, 1.0, umax, vmax);
    d3d.vertex(0.0, 1.0, umin, vmax);
}

void ImagePiece::vertice(double centerX, double centerY, double size)
{
    d3d.pushMatrix();

    d3d.translate2D(-centerX, -centerY);
    d3d.scale2D(size, size);

    d3d.setTexture(tex);
    d3d.vertex(0.0, 0.0, umin, vmin);
    d3d.vertex(1.0, 0.0, umax, vmin);
    d3d.vertex(1.0, 1.0, umax, vmax);
    d3d.vertex(0.0, 1.0, umin, vmax);

    d3d.popMatrix();
}

void ImagePiece::vertice(double centerX, double centerY, double sizeX, double sizeY)
{
    d3d.pushMatrix();

    d3d.translate2D(centerX, centerY);
    d3d.scale2D(sizeX, sizeY);

    d3d.setTexture(tex);
    d3d.vertex(0.0, 0.0, umin, vmin);
    d3d.vertex(1.0, 0.0, umax, vmin);
    d3d.vertex(1.0, 1.0, umax, vmax);
    d3d.vertex(0.0, 1.0, umin, vmax);

    d3d.popMatrix();
}
