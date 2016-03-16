#include "imagepiece.h"
#include "texturecache.h"
#include "vertexbuffer.h"

#include <cstring>

using namespace std;

vector<pair<ImagePiece*, string>>* ImagePiece::pendingList = nullptr;
bool ImagePiece::shouldLoadLater = true;

void ImagePiece::pendingLoads()
{
    shouldLoadLater = true;
}

void ImagePiece::loadPending()
{
    shouldLoadLater = false;
    if(pendingList == nullptr)
        return;
    for(auto& pair : *pendingList)
    {
        if(pair.first == nullptr)
            continue;
        pair.first->tex = TextureCache::load(pair.second.c_str());
        pair.first->loadId = -1;
    }
    delete pendingList;
    //pendingMap.clear();
}

ImagePiece::ImagePiece(Texture texture)
    : tex(texture)
    , umin(0.0)
    , vmin(0.0)
    , umax(1.0)
    , vmax(1.0)
    , loadId(-1)
{

}

ImagePiece::ImagePiece(Texture texture, double minU, double minV, double maxU, double maxV)
    : tex(texture)
    , umin(minU)
    , vmin(minV)
    , umax(maxU)
    , vmax(maxV)
    , loadId(-1)
{

}

ImagePiece::ImagePiece(const char* filename)
    : tex(0)
    , umin(0.0)
    , vmin(0.0)
    , umax(1.0)
    , vmax(1.0)
    , loadId(-1)
{
    loadOrLater(filename);
}

ImagePiece::ImagePiece(const char* filename, double minU, double minV, double maxU, double maxV)
    : tex(0)
    , umin(minU)
    , vmin(minV)
    , umax(maxU)
    , vmax(maxV)
    , loadId(-1)
{
    loadOrLater(filename);
}

ImagePiece::ImagePiece(const ImagePiece& other)
    : tex(other.tex)
    , umin(other.umin)
    , vmin(other.vmin)
    , umax(other.umax)
    , vmax(other.vmax)
    , loadId(-1)
{
    loadExistLater(other);
}

ImagePiece::ImagePiece(ImagePiece&& other)
    : tex(other.tex)
    , umin(other.umin)
    , vmin(other.vmin)
    , umax(other.umax)
    , vmax(other.vmax)
    , loadId(-1)
{
    loadExistLaterMove(std::forward<ImagePiece>(other));
}

vector<ImagePiece> ImagePiece::createImageSet(const char* filename, int row, int col, int maxIndex)
{
    return move(createImageSet(filename, row, col, maxIndex, 0, 0, 1, 1));
}

vector<ImagePiece> ImagePiece::createImageSet(const char* filename, int row, int col, int maxIndex,
                                              double minU, double minV, double maxU, double maxV)
{
    if(row <= 0 || col <= 0)
        return vector<ImagePiece>();
    vector<ImagePiece> res;
    res.reserve((maxIndex>=0 && maxIndex<row*col) ? maxIndex : row*col);
    double u0, v0, u1, v1;
    double du = (maxU-minU)/col;
    double dv = (maxV-minV)/row;
    for(int i=0; i<row; ++i)
    {
        for(int j=0; j<col; ++j)
        {
            if(maxIndex==0)
                return move(res);
            --maxIndex;
            u0 = minU + du*j;
            u1 = u0 + du;
            v0 = minV + dv*i;
            v1 = v0 + dv;
            res.push_back(ImagePiece((Texture)0, u0, v0, u1, v1));
            res.back().loadOrLater(filename);
        }
    }
    return move(res);
}

ImagePiece::~ImagePiece()
{
    tryNotLoadSelf();
}

void ImagePiece::loadOrLater(const char* filename)
{
    if(filename == nullptr)
        return;
    if(!shouldLoadLater)
        tex = TextureCache::load(filename);
    if(pendingList == nullptr)
        pendingList = new vector<pair<ImagePiece*, string>>();
    loadId = pendingList->size();
    pendingList->emplace_back(this, filename);
}

void ImagePiece::tryNotLoadSelf()
{
    if(shouldLoadLater && pendingList && loadId>=0)
    {
        (*pendingList)[loadId].first = nullptr;
        loadId=-1;
    }
}

void ImagePiece::loadExistLaterMove(ImagePiece&& other)
{
    memcpy(this, &other, sizeof(ImagePiece));
    if(shouldLoadLater && pendingList && loadId>=0)
    {
        (*pendingList)[loadId].first = this;
        other.loadId = -1;
    }
}

void ImagePiece::loadExistLater(const ImagePiece& other)
{
    memcpy(this, &other, sizeof(ImagePiece));
    if(shouldLoadLater && pendingList && loadId>=0)
    {
        loadId = pendingList->size();
        pendingList->emplace_back(this, (*pendingList)[loadId].second);
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
