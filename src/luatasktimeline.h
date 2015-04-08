#ifndef LUATASKTIMELINE_H
#define LUATASKTIMELINE_H

#include "luatask.h"
#include "bulletscene.h"

#include "luascript.h"

#include "mathhelper.h"

#include <queue>
#include <map>
using namespace std;

class LuaTaskTimeline
{
public:

    static void registerLuaFuncs();

    LuaTaskTimeline(BulletScene* bulletScene);

    ~LuaTaskTimeline();

    void setTime(double newTime);

    void push(const LuaTask& task);
    void clear();

    bool empty();

    void update(double deltaSec);

    double seekNextTask(double limitDeltaSec=M_DINF);
    double updateSingleTask(double limitDeltaSec=M_DINF);

private:

    static int lua_pushTask(lua_State* L);
    static int lua_taskDelay(lua_State* L);
    static int lua_taskDelayUntil(lua_State* L);
    static int lua_getTime(lua_State* L);

    static int lua_registerBullet(lua_State* L);
    static int lua_pushBullet(lua_State* L);
    static int lua_setCenter(lua_State* L);
    static int lua_setAngle(lua_State* L);
    static int lua_setBulletType(lua_State* L);

    static int lua_attachBullet(lua_State* L);
    static int lua_clearAttach(lua_State* L);

    static int lua_setAttachedBulletPosition(lua_State* L);
    static int lua_setAttachedBulletRotation(lua_State* L);
    static int lua_setAttachedBulletSpeed(lua_State* L);
    static int lua_setAttachedBulletAcceleration(lua_State* L);
    static int lua_setAttachedBulletRotateSpeed(lua_State* L);
    static int lua_setAttachedBulletPositionOffset(lua_State* L);
    static int lua_setAttachedBulletRotationOffset(lua_State* L);
    static int lua_setAttachedBulletSpeedOffset(lua_State* L);

    static int lua_testFunc(lua_State* L);

    static LuaTaskTimeline* instance;

    double timeSec;

    BulletScene* scene;
    LuaTask* currentTask;

    class LTComp
    {
    public:
        bool operator()(const LuaTask* a, const LuaTask* b)
        {
            return a->nextTime>b->nextTime;
        }
    };

    map<int, LuaTask> taskMap;
    priority_queue<LuaTask*, vector<LuaTask*>, LTComp> taskList;

};


#endif // LUATASKTIMELINE_H
