#ifndef IMAGEPIECE_H
#define IMAGEPIECE_H

#include "gfxdef.h"

#include <string>
#include <deque>
using namespace std;

class ImagePiece
{
public:

    static void pendingLoads();
    static void loadPending();

    ImagePiece(Texture texture=0);
    ImagePiece(Texture texture, double minU, double minV, double maxU, double maxV);

    ImagePiece(const char* filename);
    ImagePiece(const char* filename, double minU, double minV, double maxU, double maxV);

    void load(const char* filename);
    void load(const char* filename, double minU, double minV, double maxU, double maxV);

    void setTexture(Texture texture);
    void setRenderRange(double minU, double minV, double maxU, double maxV);

    // push vertices with x and y either 0.0 or 1.0
    void vertice();

    // square with edge size specified
    void vertice(double centerX, double centerY, double size=1.0);

    // rect with height and width specified
    void vertice(double centerX, double centerY, double sizeX, double sizeY);

protected:

    static deque<pair<ImagePiece*, string>> pendingList;
    static bool shouldLoadLater;

    void loadOrLater(const char* filename);

    Texture tex;

    double umin, vmin;
    double umax, vmax;

};

#endif // IMAGEPIECE_H
