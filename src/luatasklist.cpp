#include "luatasklist.h"

#include "luascript.h"
#include "debug.h"


LuaTaskList* LuaTaskList::instance = 0;
LuaTask* LuaTaskList::currentTask = 0;
BulletScene* LuaTaskList::bulletScene = 0;


LuaTaskList::LuaTaskList()
    :Ticking(1)
{
    if(!instance)
    {
        instance=this;
    }
}

LuaTaskList::~LuaTaskList()
{
    if(instance==this)
        instance=0;
}

int LuaTaskList::lua_pushTask(lua_State *L)
{
    if(!lua_isthread(L, 1))return 0;
    lua_State *t=lua_tothread(L, 1);
    lua_pushthread(t);
    int ref=luaL_ref(t, LUA_REGISTRYINDEX);
    instance->taskList.push_back(LuaTask(t, ref));
    return 0;
}

int LuaTaskList::lua_setTickRate(lua_State *L)
{
    if(lua_isnumber(L, 1))
        instance->setTickRate(lua_tonumber(L, 1));
    return 0;
}

int LuaTaskList::lua_taskDelayTicks(lua_State *L)
{
    if(lua_isnumber(L, 1)&&currentTask)
        currentTask->tickDelay+=lua_tointeger(L, 1);
    return 0;
}

int LuaTaskList::lua_testFunc(lua_State *L)
{
    bulletScene->pushBullet(new Bullet(320, 120, 1.2, 0, BULLET("rice")));
    return 0;
}

void LuaTaskList::registerLuaFuncs(lua_State *L)
{
    lua_register(L, "registerTask", lua_pushTask);
    lua_register(L, "setTickRate", lua_pushTask);
    lua_register(L, "delayTicks", lua_taskDelayTicks);
    lua_register(L, "test", lua_testFunc);
}


void LuaTaskList::onTick()
{
    int result=LUA_OK;
    list<LuaTask>::iterator iter=taskList.begin();

    while(iter!=taskList.end())
    {
        currentTask=&*iter;

        if(currentTask->tickDelay>0)
            currentTask->tickDelay--;

        if(currentTask->tickDelay<=0)
        {
            if(currentTask->luaThread)
                result=lua_resume(currentTask->luaThread, 0, 0);
        }
        else
        {
            result=LUA_YIELD;
        }

        if(result!=LUA_YIELD)
        {
            luaL_unref(_L, LUA_REGISTRYINDEX, currentTask->luaRef);
            iter=taskList.erase(iter);
        }
        else
        {
            iter++;
        }

        currentTask=0;
    }
}

