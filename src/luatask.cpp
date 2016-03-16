#include "luatask.h"

LuaTask::LuaTask()
    :luaThread(0)
    ,luaRef(-1)
    ,nextTime(0.0)
    ,isFinished(false)
    ,centerX(0.0)
    ,centerY(0.0)
    ,angle(0.0)
    ,bulletType(0)
    ,sound(0)
    ,deltaX(0.0)
    ,deltaY(0.0)
    ,centerObj(0)
{

}

LuaTask::LuaTask(lua_State* thread, int ref, double beginTime, const LuaTask* center)
    :luaThread(thread)
    ,luaRef(ref)
    ,nextTime(beginTime)
    ,isFinished(false)
    ,centerX(0.0)
    ,centerY(0.0)
    ,angle(0.0)
    ,bulletType(0)
    ,sound(0)
    ,deltaX(0.0)
    ,deltaY(0.0)
    ,centerObj(0)
{
    if(center)
    {
        centerX=center->centerX;
        centerY=center->centerY;
        angle=center->angle;
        bulletType=center->bulletType;
        deltaX=center->deltaX;
        deltaY=center->deltaY;
        centerObj=center->centerObj;
    }
}

