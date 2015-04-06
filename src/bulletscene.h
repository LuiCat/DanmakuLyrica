class BulletScene;

#ifndef BULLETSCENE_H
#define BULLETSCENE_H

#include "bullet.h"
#include "bulletlist.h"
//#include "bullettask.h"

#include <list>
#include <random>

using namespace std;

#define MAX_BULLET 8000
#define MAX_TASK 10000

class BulletScene : public Entity
{
private:

    BulletList bulletList;
    //list<LuaTask*> taskList;

public:

    double sceneCenterX, sceneCenterY;
    double sceneHeight, sceneWidth;

    mt19937 random;

    BulletScene();
    virtual ~BulletScene();

    //int getTaskSize() const;
    int getBulletSize() const;

    static Bullet* newBullet(double x, double y, double speed, double angle, int type);
    //static BulletTask* newBulletTask(lua_State *coroutine, Bullet *bullet, int paraRef=-1);

    void pushBullet(const Bullet& bullet);
    void pushBullet(double x, double y, double speed, double angle, int type);
    //LuaTask* pushTask(LuaTask *task);

    bool checkSceneBorder(Entity* entity, double offsetLength);

    //friend class LuaScript;

protected:

    void onUpdateMotion(double deltaSec, double deltaTick);
    void onRender();

    //void removeTask(list<LuaTask*>::iterator &iter);

};

#endif // BULLETSCENE_H
