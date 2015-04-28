#ifndef BULLETSCENE_H
#define BULLETSCENE_H

#include "bulletdef.h"
#include "bullet.h"
#include "bulletlist.h"

#include <list>
#include <set>
#include <random>

using namespace std;

class BulletScene : public Entity
{
private:

    BulletList bulletList;
    //list<LuaTask*> taskList;

public:

    double sceneCenterX, sceneCenterY;
    double sceneWidth, sceneHeight;
    double offsetBorderWidth;

    mt19937 random;

    BulletScene();
    virtual ~BulletScene();

    int getBulletSize() const;

    int pushBullet(const Bullet& bullet);
    int pushBullet(double x, double y, double speed, double angle, int type);

    bool checkSceneBorder(Entity* entity);

    BulletList* getBulletList();

protected:

    void onUpdateMotion(double deltaSec, double deltaTick);
    void onRender();

};

#endif // BULLETSCENE_H
