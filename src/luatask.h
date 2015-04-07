#ifndef LUATASK_H
#define LUATASK_H

#include "lua/lua.hpp"

struct LuaTask
{
    lua_State* luaThread;
    int luaRef;
    double nextTime;

    /*
     * todo:
     * bullet attach list
     * center x/y/angle
     * bullet type, sound type, etc.
     */

    LuaTask(lua_State* thread, int ref, double beginTime)
        :luaThread(thread)
        ,luaRef(ref)
        ,nextTime(beginTime)
    {

    }

    LuaTask()
        :luaThread(0)
        ,luaRef(-1)
        ,nextTime(0.0)
    {

    }

};

#endif // LUATASK_H
