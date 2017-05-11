#include "texturepiece.h"
#include "texturecache.h"
#include "shape2d.h"

#include <cstring>

using namespace std;

vector<pair<TexturePiece*, string>>* TexturePiece::pendingList = nullptr;
bool TexturePiece::shouldLoadLater = true;

void TexturePiece::pendingLoads()
{
    shouldLoadLater = true;
}

void TexturePiece::loadPending()
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

TexturePiece::TexturePiece(Texture texture)
    : tex(texture)
    , umin(0.0)
    , vmin(0.0)
    , umax(1.0)
    , vmax(1.0)
    , loadId(-1)
{

}

TexturePiece::TexturePiece(Texture texture, double minU, double minV, double maxU, double maxV)
    : tex(texture)
    , umin(minU)
    , vmin(minV)
    , umax(maxU)
    , vmax(maxV)
    , loadId(-1)
{

}

TexturePiece::TexturePiece(const char* filename)
    : tex(0)
    , umin(0.0)
    , vmin(0.0)
    , umax(1.0)
    , vmax(1.0)
    , loadId(-1)
{
    loadOrLater(filename);
}

TexturePiece::TexturePiece(const char* filename, double minU, double minV, double maxU, double maxV)
    : tex(0)
    , umin(minU)
    , vmin(minV)
    , umax(maxU)
    , vmax(maxV)
    , loadId(-1)
{
    loadOrLater(filename);
}

TexturePiece::TexturePiece(const TexturePiece& other)
    : tex(other.tex)
    , umin(other.umin)
    , vmin(other.vmin)
    , umax(other.umax)
    , vmax(other.vmax)
    , loadId(-1)
{
    loadExistLater(other);
}

TexturePiece::TexturePiece(TexturePiece&& other)
    : tex(other.tex)
    , umin(other.umin)
    , vmin(other.vmin)
    , umax(other.umax)
    , vmax(other.vmax)
    , loadId(-1)
{
    loadExistLaterMove(std::forward<TexturePiece>(other));
}

vector<TexturePiece> TexturePiece::createImageSet(const char* filename, int row, int col, int maxIndex)
{
    return move(createImageSet(filename, row, col, maxIndex, 0, 0, 1, 1));
}

vector<TexturePiece> TexturePiece::createImageSet(const char* filename, int row, int col, int maxIndex,
                                              double minU, double minV, double maxU, double maxV)
{
    if(row <= 0 || col <= 0)
        return vector<TexturePiece>();
    vector<TexturePiece> res;
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
            res.push_back(TexturePiece((Texture)0, u0, v0, u1, v1));
            res.back().loadOrLater(filename);
        }
    }
    return move(res);
}

TexturePiece::~TexturePiece()
{
    tryNotLoadSelf();
}

void TexturePiece::loadOrLater(const char* filename)
{
    if(filename == nullptr)
        return;
    if(!shouldLoadLater)
        tex = TextureCache::load(filename);
    if(pendingList == nullptr)
        pendingList = new vector<pair<TexturePiece*, string>>();
    loadId = pendingList->size();
    pendingList->emplace_back(this, filename);
}

void TexturePiece::tryNotLoadSelf()
{
    if(shouldLoadLater && pendingList && loadId>=0)
    {
        (*pendingList)[loadId].first = nullptr;
        loadId=-1;
    }
}

void TexturePiece::loadExistLaterMove(TexturePiece&& other)
{
    memcpy(this, &other, sizeof(TexturePiece));
    if(shouldLoadLater && pendingList && loadId>=0)
    {
        (*pendingList)[loadId].first = this;
        other.loadId = -1;
    }
}

void TexturePiece::loadExistLater(const TexturePiece& other)
{
    memcpy(this, &other, sizeof(TexturePiece));
    if(shouldLoadLater && pendingList && loadId>=0)
    {
        pendingList->emplace_back(this, (*pendingList)[loadId].second);
        loadId = pendingList->size() - 1;
    }
}

void TexturePiece::load(const char* filename)
{
    loadOrLater(filename);
    umin = 0.0;
    vmin = 0.0;
    umax = 1.0;
    vmax = 1.0;
}

void TexturePiece::load(const char* filename, double minU, double minV, double maxU, double maxV)
{
    loadOrLater(filename);
    umin = minU;
    vmin = minV;
    umax = maxU;
    vmax = maxV;
}

void TexturePiece::setTexture(Texture texture)
{
    tex = texture;
}

void TexturePiece::setRenderRange(double minU, double minV, double maxU, double maxV)
{
    umin = minU;
    vmin = minV;
    umax = maxU;
    vmax = maxV;
}

void TexturePiece::vertice()
{
    d2d.setTexture(tex);
    d2d.rect(umin, vmin, umax, vmax);
}

void TexturePiece::vertice(double centerXRatio, double centerYRatio, double size)
{
    d2d.pushMatrix();

    //d2d.scale(size, size);
    //d2d.translate(-centerXRatio, -centerYRatio);
    double offsetX = -size*centerXRatio;
    double offsetY = -size*centerYRatio;

    d2d.setTexture(tex);
    d2d.rect(offsetX, offsetY, size + offsetX, size + offsetY, umin, vmin, umax, vmax);

    d2d.popMatrix();
}

void TexturePiece::vertice(double centerXRatio, double centerYRatio, double sizeX, double sizeY)
{
    d2d.pushMatrix();

    //d2d.scale(sizeX, sizeY);
    //d2d.translate(-centerXRatio, -centerYRatio);

    double offsetX = -sizeX*centerXRatio;
    double offsetY = -sizeY*centerYRatio;

    d2d.setTexture(tex);
    d2d.rect(offsetX, offsetY, sizeX + offsetX, sizeY + offsetY, umin, vmin, umax, vmax);

    d2d.popMatrix();
}
