#ifndef TEXTUREPIECE_H
#define TEXTUREPIECE_H

#include "gfxdef.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <deque>
using namespace std;

class TexturePiece
{
public:

    static void pendingLoads();
    static void loadPending();

    TexturePiece(Texture texture=0);
    TexturePiece(Texture texture, double minU, double minV, double maxU, double maxV);

    TexturePiece(const char* filename);
    TexturePiece(const char* filename, double minU, double minV, double maxU, double maxV);

    TexturePiece(const TexturePiece& other);
    TexturePiece(TexturePiece&& other);

    // create multiple image piece. indexed row by row.
    static std::vector<TexturePiece> createImageSet(const char* filename, int row, int col, int maxIndex=-1);
    static std::vector<TexturePiece> createImageSet(const char* filename, int row, int col, int maxIndex,
                                                  double minU, double minV, double maxU, double maxV);

    ~TexturePiece();

    void load(const char* filename);
    void load(const char* filename, double minU, double minV, double maxU, double maxV);

    void setTexture(Texture texture);
    void setRenderRange(double minU, double minV, double maxU, double maxV);

    inline TexturePiece& operator=(const TexturePiece& other)
    {
        loadExistLater(other);
        return *this;
    }

    inline TexturePiece& operator=(TexturePiece&& other)
    {
        loadExistLaterMove(std::forward<TexturePiece>(other));
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

    static vector<pair<TexturePiece*, string>>* pendingList;
    static bool shouldLoadLater;

    void loadOrLater(const char* filename);
    void tryNotLoadSelf();
    void loadExistLater(const TexturePiece& other);
    void loadExistLaterMove(TexturePiece&& other);

    Texture tex;

    double umin, vmin;
    double umax, vmax;

    int loadId;

};

#endif // TEXTUREPIECE_H
