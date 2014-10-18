#include "bulletscene.h"

#include "debug.h"

BulletScene::BulletScene()
    :Entity()
    ,sceneCenterX(320.0)
    ,sceneCenterY(240.0)
    ,sceneHeight(480.0)
    ,sceneWidth(640.0)
{
    useDefaultTicking=false;
}

BulletScene::~BulletScene()
{
    //taskList.clear();
    bulletList.clearAll();
}

Bullet* BulletScene::newBullet(double x, double y, double speed, double angle, int type)
{
    return new Bullet(x, y, speed, angle, type);
}

/*
BulletTask* BulletScene::newBulletTask(lua_State* coroutine, Bullet *bullet, int paraRef)
{
    return new BulletTask(coroutine, bullet, paraRef);
}
*/

Bullet* BulletScene::pushBullet(Bullet *bullet)
{
    bulletList.pushEntity(bullet);
    return bullet;
}

/*
LuaTask* BulletScene::pushTask(LuaTask *task)
{
    if(taskList.size()>=MAX_TASK)return 0;
    taskList.push_back(task);
    return taskList.back();
}
*/

bool BulletScene::checkSceneBorder(Entity *entity, double offsetLength)
{
    double px=entity->getX(), py=entity->getY();
    double sx=entity->getSpeedX(), sy=entity->getSpeedY();
    return (sx*(px-sceneCenterX)*2>abs(sx)*(sceneWidth+offsetLength)||
            sy*(py-sceneCenterY)*2>abs(sy)*(sceneHeight+offsetLength));
}

/*
int BulletScene::getTaskSize() const
{
    return taskList.size();
}
*/

int BulletScene::getBulletSize() const
{
    return bulletList.size();
}

void BulletScene::onUpdateMotion(double deltaSec, double)
{
    //double d=deltaSec*1.1+(sin(timeSec+deltaSec)-sin(timeSec));

    /*
    for(auto x : taskList)
    {
        x->update(deltaSec);
    }
    */

    bulletList.updateAll(deltaSec);

    /*
    auto iter_task=taskList.begin();
    while(iter_task!=taskList.end())
    {
        if((*iter_task)->dead())
            removeTask(iter_task);
        else
            ++iter_task;
    }
    */
}

void BulletScene::onRender()
{
    bulletList.renderAll();
}

/*
void BulletScene::removeTask(list<LuaTask*>::iterator &iter)
{
    if(*iter)
    {
        (*iter)->release();
        delete *iter;
    }
    iter=taskList.erase(iter);
}
*/
