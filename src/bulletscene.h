#ifndef BULLETSCENE_H
#define BULLETSCENE_H

#include "bulletdef.h"
#include "bullet.h"
#include "bulletlist.h"
#include "scene.h"
#include "player.h"

#include <list>
#include <set>
#include <random>

using namespace std;

class BulletScene : public Scene
{
private:

    BulletList bulletList;
    //list<LuaTask*> taskList;

    Player* player;

public:

    double sceneCenterX, sceneCenterY;
    double sceneWidth, sceneHeight;
    double offsetBorderWidth;

    mt19937 random;

    BulletScene();
    virtual ~BulletScene();

    int pushBullet(const Bullet& bullet);
    int pushBullet(double x, double y, double speed, double angle, int type);

    bool checkSceneBorder(Entity* entity);

    void setPlayerMotion(PlayerDirection dir);

    int getBulletSize() const;
    BulletList* getBulletList();

    void load();
    void unload();

    void update(rtime_t deltaTime);
    void render();

};

#endif // BULLETSCENE_H
