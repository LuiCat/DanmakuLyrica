#ifndef LUATASKTIMELINE_H
#define LUATASKTIMELINE_H

#include "luatask.h"
#include "bulletscene.h"

#include "luascript.h"

#include <queue>
using namespace std;

class LuaTaskTimeline
{
public:

    static void registerLuaFuncs();

    LuaTaskTimeline(BulletScene* bulletScene);

    ~LuaTaskTimeline();

    void push(const LuaTask& task);
    void clear();

    void update(double deltaSec);
    double updateSingleTask(double limitDeltaSec);

private:

    static int lua_pushTask(lua_State* L);
    static int lua_taskDelaySecs(lua_State* L);

    static int lua_testFunc(lua_State* L);

    double timeSec;

    BulletScene* scene;

    static LuaTaskTimeline* instance;
    static LuaTask currentTask;

    priority_queue<LuaTask> taskList;


};

#endif // LUATASKTIMELINE_H
