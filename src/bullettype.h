#ifndef BULLETTYPE_H
#define BULLETTYPE_H

#include "texturestrip.h"

#include <imagepiece.h>

struct BulletType
{
    ImagePiece image;

    /*
    Texture texture;
*/

    double sizeX, sizeY;
    double centerX, centerY;

    enum FacingType
    {
        none, forward, spin
    }facingType;

    enum JudgeType
    {
        round, rect, oval
    }judgeType;

    double judgeSizeX, judgeSizeY;
};

#endif // BULLETTYPE_H

