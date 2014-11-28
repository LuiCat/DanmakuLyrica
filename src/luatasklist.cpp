#include "luatasklist.h"

#include "luascript.h"
#include "debug.h"

LuaTaskList* LuaTaskList::instance = 0;
LuaTaskList::Task* LuaTaskList::currentTask = 0;

LuaTaskList::LuaTaskList()
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
    if(lua_isthread(L, 1))
        instance->taskList.push_back(Task(lua_tothread(L, 1), luaL_ref(L, LUA_REGISTRYINDEX)));
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
    cout<<"ayayaya"<<endl;
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
    bool removeFlag;
    list<Task>::iterator iter=taskList.begin();

    while(iter!=taskList.end())
    {
        removeFlag=false;
        currentTask=&*iter;

        if(currentTask->tickDelay<=0)
        {
            lua_getglobal(_L, "resumeTask");
            lua_pushthread(currentTask->luaThread);

            LuaScript::lua_excall(1, 1);

            removeFlag=!lua_toboolean(_L, -1);
            lua_pop(_L, 1);

        }
        else
        {
            currentTask->tickDelay--;
        }

        if(removeFlag)
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

