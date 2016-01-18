#include "imagepiece.h"
#include "texturecache.h"
#include "vertexbuffer.h"

#include <cstring>

vector<pair<ImagePiece*, string>> ImagePiece::pendingList;
unordered_map<const ImagePiece*, int>   ImagePiece::pendingMap;
bool ImagePiece::shouldLoadLater = true;

void ImagePiece::pendingLoads()
{
    shouldLoadLater = true;
}

void ImagePiece::loadPending()
{
    shouldLoadLater = false;
    for(auto& pair : pendingList)
    {
        if(pair.first == nullptr)
            continue;
        pair.first->tex = TextureCache::load(pair.second.c_str());
    }
    pendingList.clear();\
    pendingMap.clear();
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

ImagePiece::ImagePiece(const ImagePiece& other)
    : tex(other.tex)
    , umin(other.umin)
    , vmin(other.vmin)
    , umax(other.umax)
    , vmax(other.vmax)
{
    loadExistLater(other);
}

ImagePiece::ImagePiece(ImagePiece&& other)
    : tex(other.tex)
    , umin(other.umin)
    , vmin(other.vmin)
    , umax(other.umax)
    , vmax(other.vmax)
{
    loadExistLater(other, true);
}

ImagePiece::~ImagePiece()
{
    tryNotLoadSelf();
}

void ImagePiece::loadOrLater(const char* filename)
{
    if(!shouldLoadLater)
        tex = TextureCache::load(filename);
    pendingMap[this]=pendingList.size();
    pendingList.emplace_back(this, filename);
}

void ImagePiece::tryNotLoadSelf()
{
    if(shouldLoadLater && pendingMap.count(this))
    {
        auto iter = pendingMap.find(this);
        if(iter != pendingMap.end())
        {
            pendingList[iter->second].first = nullptr;
            pendingMap.erase(iter);
        }
    }
}

void ImagePiece::loadExistLater(const ImagePiece& other, bool move)
{
    memcpy(this, &other, sizeof(ImagePiece));
    if(shouldLoadLater)
    {
        auto iter = pendingMap.find(&other);
        if(move)
        {
            pendingMap[this]=iter->second;
            pendingList[iter->second].first = this;
            pendingMap.erase(iter);
        }
        else if(iter != pendingMap.end())
        {
            pendingMap[this]=pendingList.size();
            pendingList.emplace_back(this, pendingList[iter->second].second);
        }
    }
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

ImagePiece& ImagePiece::operator=(const ImagePiece& other)
{
    loadExistLater(other);
    return *this;
}

ImagePiece& ImagePiece::operator=(ImagePiece&& other)
{
    loadExistLater(other, true);
    return *this;
}

void ImagePiece::vertice()
{
    d3d.setTexture(tex);
    d3d.vertex(0.0, 0.0, umin, vmin);
    d3d.vertex(1.0, 0.0, umax, vmin);
    d3d.vertex(1.0, 1.0, umax, vmax);
    d3d.vertex(0.0, 1.0, umin, vmax);
}

void ImagePiece::vertice(double centerXRatio, double centerYRatio, double size)
{
    d3d.pushMatrix();

    d3d.scale2D(size, size);
    d3d.translate2D(-centerXRatio, -centerYRatio);

    d3d.setTexture(tex);
    d3d.vertex(0.0, 0.0, umin, vmin);
    d3d.vertex(1.0, 0.0, umax, vmin);
    d3d.vertex(1.0, 1.0, umax, vmax);
    d3d.vertex(0.0, 1.0, umin, vmax);

    d3d.popMatrix();
}

void ImagePiece::vertice(double centerXRatio, double centerYRatio, double sizeX, double sizeY)
{
    d3d.pushMatrix();

    d3d.scale2D(sizeX, sizeY);
    d3d.translate2D(-centerXRatio, -centerYRatio);

    d3d.setTexture(tex);
    d3d.vertex(0.0, 0.0, umin, vmin);
    d3d.vertex(1.0, 0.0, umax, vmin);
    d3d.vertex(1.0, 1.0, umax, vmax);
    d3d.vertex(0.0, 1.0, umin, vmax);

    d3d.popMatrix();
}
