#ifndef IMAGEPIECE_H
#define IMAGEPIECE_H

#include "gfxdef.h"

#include <unordered_map>
#include <string>
#include <vector>
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

    ImagePiece(const ImagePiece& other);
    ImagePiece(ImagePiece&& other);

    // create multiple image piece. indexed row by row.
    static std::vector<ImagePiece> createImageSet(const char* filename, int row, int col, int maxIndex=-1);
    static std::vector<ImagePiece> createImageSet(const char* filename, int row, int col, int maxIndex,
                                                  double minU, double minV, double maxU, double maxV);

    ~ImagePiece();

    void load(const char* filename);
    void load(const char* filename, double minU, double minV, double maxU, double maxV);

    void setTexture(Texture texture);
    void setRenderRange(double minU, double minV, double maxU, double maxV);

    inline ImagePiece& operator=(const ImagePiece& other)
    {
        loadExistLater(other);
        return *this;
    }

    inline ImagePiece& operator=(ImagePiece&& other)
    {
        loadExistLaterMove(std::forward<ImagePiece>(other));
        return *this;
    }

    // push vertices with x and y either 0.0 or 1.0
    void vertice();

    // square with edge size specified. center is given by ratio (between 0.0 and 1.0, can be <0.0 or >1.0)
    void vertice(double centerXRatio, double centerYRatio, double size=1.0);

    // rect with height and width specified
    void vertice(double centerXRatio, double centerYRatio, double sizeX, double sizeY);

    inline Texture texture() const
    {
        return tex;
    }

protected:

    static vector<pair<ImagePiece*, string>>* pendingList;
    static bool shouldLoadLater;

    void loadOrLater(const char* filename);
    void tryNotLoadSelf();
    void loadExistLater(const ImagePiece& other);
    void loadExistLaterMove(ImagePiece&& other);

    Texture tex;

    double umin, vmin;
    double umax, vmax;

    int loadId;

};

#endif // IMAGEPIECE_H
