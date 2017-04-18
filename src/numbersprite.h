#ifndef NUMBERSPRITE_H
#define NUMBERSPRITE_H

#include "renderobject.h"
#include "texturepiece.h"

#include <vector>
#include <string>
using namespace std;

class NumberSprite : public RenderObject
{
public:

    NumberSprite();
    NumberSprite(const char* filename, int row, int col);

    void loadImage(const char* filename, int row, int col);

    void setNumber(int x);
    void setFormat(const char* format);
    void setMaxChar(int maxc);

    void setCharSize(double height, double width, double spaceRatio = 1.0);

    enum Alignment
    {
        AlignLeft,
        AlignRight,
        AlignCenter,
    };

    void setAlignment(Alignment align);

protected:

    void formatContent();

    void onRender();

    vector<TexturePiece> imageNum;

    int num;
    int maxbuf;
    string format;
    string content;

    double h;
    double w;
    double k;

    Alignment align;

};

#endif // NUMBERSPRITE_H
