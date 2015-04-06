#include "luatasktimeline.h"

#include "mathhelper.h"

LuaTaskTimeline* LuaTaskTimeline::instance = 0;
LuaTask LuaTaskTimeline::currentTask;

void LuaTaskTimeline::registerLuaFuncs()
{
    lua_register(_L, "pushTask", lua_pushTask);
    lua_register(_L, "delaySecs", lua_taskDelaySecs);

    lua_register(_L, "test", lua_testFunc);
}

int LuaTaskTimeline::lua_pushTask(lua_State* L)
{
    if(!lua_isthread(L, 1))return 0;
    lua_State *t=lua_tothread(L, 1);
    lua_pushthread(t);
    int ref=luaL_ref(t, LUA_REGISTRYINDEX);
    instance->push(LuaTask(t, ref, instance->timeSec));
    return 0;
}

int LuaTaskTimeline::lua_taskDelaySecs(lua_State* L)
{
    if(lua_isnumber(L, 1))
        currentTask.nextTime+=lua_tonumber(L, 1);
    return 0;
}

int LuaTaskTimeline::lua_testFunc(lua_State* L)
{
    static int t=0;
    double w=M_PI*167.00/120;
    double a=17.2;
    double r=a/w/w;
    Bullet b(320, 240, 0, 0, BULLET("rice"));
    b.setAcceleration(a).setRotateSpeed(w);
    instance->scene->pushBullet(b);
    t++;
    return 0;
}

LuaTaskTimeline::LuaTaskTimeline(BulletScene* bulletScene)
{
    instance=this;
    scene=bulletScene;
}

LuaTaskTimeline::~LuaTaskTimeline()
{
    clear();
}

void LuaTaskTimeline::push(const LuaTask& task)
{
    taskList.push(task);
}

void LuaTaskTimeline::update(double deltaSec)
{
    double newSec=timeSec+deltaSec;

    int result=LUA_OK;

    while(true)
    {
        currentTask=taskList.top();

        if(currentTask.nextTime>newSec)
            break;

        taskList.pop();
        timeSec=currentTask.nextTime;

        result=LUA_OK;

        if(currentTask.luaThread)
            result=lua_resume(currentTask.luaThread, 0, 0);

        if(result!=LUA_YIELD)
        {
            luaL_unref(_L, LUA_REGISTRYINDEX, currentTask.luaRef);
            continue;
        }

        taskList.push(currentTask);

    }

    timeSec=newSec;

}

double LuaTaskTimeline::updateSingleTask(double limitDeltaSec)
{
    double newSec=timeSec+limitDeltaSec;

    int result=LUA_OK;

    currentTask=taskList.top();

    if(currentTask.nextTime>newSec)
    {
        timeSec=newSec;
        return 0.0;
    }

    taskList.pop();
    timeSec=currentTask.nextTime;

    result=LUA_OK;

    if(currentTask.luaThread)
        result=lua_resume(currentTask.luaThread, 0, 0);

    if(result!=LUA_YIELD)
    {
        luaL_unref(_L, LUA_REGISTRYINDEX, currentTask.luaRef);
    }
    else
    {
        taskList.push(currentTask);
    }

    return newSec-timeSec;
}

void LuaTaskTimeline::clear()
{
    while(!taskList.empty())
    {
        if(_L)
        {
            const LuaTask& t=taskList.top();
            luaL_unref(_L, LUA_REGISTRYINDEX, t.luaRef);
        }
        taskList.pop();
    }
}


