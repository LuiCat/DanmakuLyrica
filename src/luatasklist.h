#ifndef LUATASKLIST_H
#define LUATASKLIST_H

#include "ticking.h"
#include "lua/lua.hpp"

#include <list>
using namespace std;

class LuaTaskList : public Ticking
{
public:

    static void registerLuaFuncs(lua_State* L);

    LuaTaskList();
    ~LuaTaskList();


protected:

    struct Task
    {
        lua_State* luaThread;
        int luaRef;
        int tickDelay;
        Task(lua_State* thread=0, int ref=-1)
            :luaThread(thread)
            ,luaRef(ref)
            ,tickDelay(0)
        {}
    };

    static LuaTaskList* instance;
    static Task* currentTask;

    static int lua_pushTask(lua_State* L);
    static int lua_setTickRate(lua_State* L);
    static int lua_taskDelayTicks(lua_State* L);

    static int lua_testFunc(lua_State* L);


    list<Task> taskList;

    void onTick();

};

#endif // LUATASKLIST_H
