#include "luatask.h"

LuaTask::LuaTask()
    :luaThread(0)
    ,luaRef(-1)
    ,nextTime(0.0)
    ,centerX(0.0)
    ,centerY(0.0)
    ,angle(0.0)
    ,bulletType(0)
{

}

LuaTask::LuaTask(lua_State* thread, int ref, double beginTime, const LuaTask* center)
    :luaThread(thread)
    ,luaRef(ref)
    ,nextTime(beginTime)
    ,centerX(0.0)
    ,centerY(0.0)
    ,angle(0.0)
    ,bulletType(0)
{
    if(center)
    {
        centerX=center->centerX;
        centerY=center->centerY;
        angle=center->angle;
        bulletType=center->bulletType;
    }
}

