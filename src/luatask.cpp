
#include "luatask.h"

LuaTask::LuaTask()
    :luaThread(0)
    ,luaRef(-1)
    ,nextTime(0.0)
{

}

LuaTask::LuaTask(lua_State* thread, int ref, double beginTime)
    :luaThread(thread)
    ,luaRef(ref)
    ,nextTime(beginTime)
{

}


