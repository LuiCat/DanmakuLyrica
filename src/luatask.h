#ifndef LUATASK_H
#define LUATASK_H

#include "lua/lua.hpp"

struct LuaTask
{
    lua_State* luaThread;
    int luaRef;
    int tickDelay;


    LuaTask(lua_State* thread=0, int ref=-1)
        :luaThread(thread)
        ,luaRef(ref)
        ,tickDelay(0)
    {

    }

};

#endif // LUATASK_H
