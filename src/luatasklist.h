#ifndef LUATASKLIST_H
#define LUATASKLIST_H

#include "ticking.h"
#include "luatask.h"

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

    static LuaTaskList* instance;
    static LuaTask* currentTask;

    static int lua_pushTask(lua_State* L);
    static int lua_setTickRate(lua_State* L);
    static int lua_taskDelayTicks(lua_State* L);

    static int lua_testFunc(lua_State* L);


    list<LuaTask> taskList;

    void onTick();

};

#endif // LUATASKLIST_H
