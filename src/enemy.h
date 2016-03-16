#ifndef ENEMY_H
#define ENEMY_H

#include "bulletbase.h"

#include "imagepiece.h"
#include <vector>

using namespace std;


class Enemy : public BulletBase
{
public:

    Enemy();

    ExtendType getType() const
    {
        return Type_Enemy;
    }

protected:

    vector<ImagePiece> img;
    ImagePiece wheel;

    void onTick();
    void onRender();
    void onDestroy();

    bool isOutsideScene(double x1, double y1, double x2, double y2);



};

#endif // ENEMY_H
