#include "luatimeline.h"

#include "bulletbase.h"
#include "bullet.h"

#include "mathhelper.h"
#include "debug.h"

//===========================================================

void LuaTimeline::registerLuaFuncs()
{
    registerFunc("pushTask", lua_pushTask);
    registerFunc("delay", lua_taskDelay);
    registerFunc("delayUntil", lua_taskDelayUntil);
    registerFunc("getTime", lua_getTime);
}

int LuaTimeline::lua_pushTask(lua_State* L)
{
    if(!lua_isthread(L, 1))return 0;
    lua_State *t=lua_tothread(L, 1);
    lua_pushthread(t);
    int ref=luaL_ref(t, LUA_REGISTRYINDEX);
    inst()->push(LuaTask(t, ref, inst()->timeSec, inst()->currentTask));
    lua_pushinteger(L, ref);
    return 1;
}

int LuaTimeline::lua_taskDelay(lua_State* L)
{
    if(lua_isnumber(L, 1))
        inst()->currentTask->delay(lua_tonumber(L, 1));
    return 0;
}

int LuaTimeline::lua_taskDelayUntil(lua_State* L)
{
    if(lua_isnumber(L, 1))
        inst()->currentTask->delayUntil(lua_tonumber(L, 1));
    return 0;
}

int LuaTimeline::lua_getTime(lua_State* L)
{
    lua_pushnumber(L, inst()->timeSec);
    return 1;
}

bool LuaTimeline::beforeTask(LuaTask* task)
{
    return !task->isFinished;
}

//===========================================================

LuaTimeline::LuaTimeline()
    :timeSec(0.0)
    ,currentTask(0)
{
    registerLuaFuncs();
}

LuaTimeline::~LuaTimeline()
{
    clearTask();
}

void LuaTimeline::setTime(double newTime)
{
    timeSec=newTime;
}

void LuaTimeline::push(const LuaTask& task)
{
    taskMap[task.luaRef]=task;
    taskList.push(&taskMap[task.luaRef]);
}

void LuaTimeline::update(double deltaSec)
{
    double newSec=timeSec+deltaSec;

    int result=LUA_OK;

    while(!taskList.empty())
    {
        currentTask=taskList.top();

        if(currentTask->nextTime>newSec)
            break;

        taskList.pop();
        timeSec=currentTask->nextTime;

        result=LUA_OK;

        if(currentTask->luaThread)
        {
            beginScript();
            if(beforeTask(currentTask))
                result=lua_resume(currentTask->luaThread, 0, 0);
            endScript();
        }

        if(result!=LUA_YIELD || currentTask->isFinished)
        {
            luaL_unref(luaState, LUA_REGISTRYINDEX, currentTask->luaRef);
            taskMap.erase(currentTask->luaRef);
            continue;
        }

        taskList.push(currentTask);

    }

    timeSec=newSec;

}

double LuaTimeline::seekNextTask(double limitDeltaSec)
{
    if(taskList.empty())
    {
        timeSec+=limitDeltaSec;
        return 0.0;
    }

    double newSec=timeSec+limitDeltaSec;
    LuaTask* task=taskList.top();

    if(task->nextTime>newSec)
    {
        timeSec=newSec;
        return 0.0;
    }

    if(task->nextTime>timeSec)
        timeSec=task->nextTime;

    return newSec-timeSec;

}

double LuaTimeline::updateSingleTask(double limitDeltaSec)
{
    if(taskList.empty())
    {
        timeSec+=limitDeltaSec;
        return 0.0;
    }

    double newSec=timeSec+limitDeltaSec;

    int result=LUA_OK;

    currentTask=taskList.top();

    if(currentTask->nextTime>newSec)
    {
        timeSec=newSec;
        return 0.0;
    }

    taskList.pop();
    timeSec=currentTask->nextTime;

    result=LUA_OK;

    if(currentTask->luaThread)
    {
        beginScript();
        if(beforeTask(currentTask))
            result=lua_resume(currentTask->luaThread, 0, 0);
        endScript();
    }

    if(result!=LUA_YIELD || currentTask->isFinished)
    {
        luaL_unref(luaState, LUA_REGISTRYINDEX, currentTask->luaRef);
        taskMap.erase(currentTask->luaRef);
    }
    else
    {
        taskList.push(currentTask);
    }

    return newSec-timeSec;
}

void LuaTimeline::clearTask()
{
    while(!taskList.empty())
    {
        luaL_unref(luaState, LUA_REGISTRYINDEX, taskList.top()->luaRef);
        taskList.pop();
    }
    taskMap.clear();
}

bool LuaTimeline::taskListEmpty()
{
    return taskList.empty();
}


