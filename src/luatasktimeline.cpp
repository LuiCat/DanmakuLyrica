#include "luatasktimeline.h"

#include "bullet.h"

#include "mathhelper.h"
#include "debug.h"

LuaTaskTimeline* LuaTaskTimeline::instance = 0;

void LuaTaskTimeline::registerLuaFuncs()
{
    lua_register(_L, "pushTask", lua_pushTask);
    lua_register(_L, "delay", lua_taskDelay);
    lua_register(_L, "delayUntil", lua_taskDelayUntil);
    lua_register(_L, "getTime", lua_getTime);

    lua_register(_L, "registerBullet", lua_registerBullet);
    lua_register(_L, "pushBullet", lua_pushBullet);
    lua_register(_L, "setCenter", lua_setCenter);
    lua_register(_L, "setAngle", lua_setAngle);
    lua_register(_L, "setBulletType", lua_setBulletType);
    /*
    static int lua_registerBullet(lua_State* L);
    static int lua_pushBullet(lua_State* L);
    static int lua_setCenter(lua_State* L);
    static int lua_setAngle(lua_State* L);
    static int lua_setBulletType(lua_State* L);
    */

    //lua_register(_L, "test", lua_testFunc);
}

int LuaTaskTimeline::lua_pushTask(lua_State* L)
{
    if(!lua_isthread(L, 1))return 0;
    lua_State *t=lua_tothread(L, 1);
    lua_pushthread(t);
    int ref=luaL_ref(t, LUA_REGISTRYINDEX);
    instance->push(LuaTask(t, ref, instance->timeSec, instance->currentTask));
    lua_pushinteger(L, ref);
    return 1;
}

int LuaTaskTimeline::lua_taskDelay(lua_State* L)
{
    if(lua_isnumber(L, 1))
        instance->currentTask->nextTime+=lua_tonumber(L, 1);
    return 0;
}

int LuaTaskTimeline::lua_taskDelayUntil(lua_State* L)
{
    if(lua_isnumber(L, 1))
        instance->currentTask->nextTime=lua_tonumber(L, 1);
    return 0;
}

int LuaTaskTimeline::lua_getTime(lua_State* L)
{
    lua_pushnumber(L, instance->timeSec);
    return 1;
}

int LuaTaskTimeline::lua_registerBullet(lua_State* L)
{
    if(lua_gettop(L)!=10)return 0;

    if(!lua_isstring(L, 1))return 0;

    BulletType type;
    char buffer[256];
    const char *str;

    strcpy(buffer, LuaScript::getCurrentPath());
    strcat(buffer, lua_tostring(L, 2));
    type.texture=0;
    if(createTexture(buffer, &type.texture)!=D3D_OK)return 0;

    type.sizeX=lua_tonumber(L, 3);
    type.sizeY=lua_tonumber(L, 4);
    type.centerX=lua_tonumber(L, 5);
    type.centerY=lua_tonumber(L, 6);

    if(lua_isstring(L, 7))
    {
        str=lua_tostring(L, 7);
        if(strcmp(str, "spin")==0||strcmp(str, "Spin")==0)
        {
            type.facingType=BulletType::spin;
        }
        else if(strcmp(str, "forward")==0||strcmp(str, "Forward")==0)
        {
            type.facingType=BulletType::forward;
        }
        else
        {
            type.facingType=BulletType::none;
        }
    }
    else
    {
        type.facingType=(BulletType::FacingType)lua_tointeger(L, 7);
    }

    if(lua_isstring(L, 7))
    {
        str=lua_tostring(L, 7);
        if(strcmp(str, "oval")==0||strcmp(str, "Oval")==0)
        {
            type.judgeType=BulletType::oval;
        }
        else if(strcmp(str, "rect")==0||strcmp(str, "Rect")==0)
        {
            type.judgeType=BulletType::rect;
        }
        else
        {
            type.judgeType=BulletType::round;
        }
    }
    else
    {
        type.judgeType=(BulletType::JudgeType)lua_tointeger(L, 8);
    }

    type.judgeSizeX=lua_tonumber(L, 9);
    type.judgeSizeY=lua_tonumber(L, 10);

    lua_pushinteger(L, Bullet::reg(lua_tostring(L, 1), &type));

    cout<<"Bullet \""<<lua_tostring(L, 1)<<"\" Registered, texture id "<<type.texture<<endl;

    return 1;
}

int LuaTaskTimeline::lua_pushBullet(lua_State* L)
{
    if(lua_gettop(L)<4)
        return 0;

    double x=lua_tonumber(L, 1)+instance->currentTask->centerX;
    double y=lua_tonumber(L, 2)+instance->currentTask->centerY;
    double sp=lua_tonumber(L, 3);
    double rt=rad(lua_tonumber(L, 4))+instance->currentTask->angle;
    double type=(lua_isstring(L, 5)?BULLET(lua_tostring(L, 5)):lua_isnumber(L, 5)?lua_tointeger(L, 5):instance->currentTask->bulletType);

    lua_pushinteger(L, instance->scene->pushBullet(x, y, sp, rt, type));

    return 1;
}

int LuaTaskTimeline::lua_setCenter(lua_State* L)
{
    if(lua_isnumber(L, 1))
        instance->currentTask->centerX=lua_tonumber(L, 1);
    if(lua_isnumber(L, 2))
        instance->currentTask->centerY=lua_tonumber(L, 2);
    return 0;
}

int LuaTaskTimeline::lua_setAngle(lua_State* L)
{
    if(lua_isnumber(L, 1))
        instance->currentTask->angle=lua_tonumber(L, 1);
    return 0;
}

int LuaTaskTimeline::lua_setBulletType(lua_State* L)
{
    if(lua_isnumber(L, 1))
        instance->currentTask->bulletType=lua_tointeger(L, 1);
    else if(lua_isstring(L, 1))
        instance->currentTask->bulletType=BULLET(lua_tostring(L, 1));
    return 0;
}

int LuaTaskTimeline::lua_testFunc(lua_State* L)
{
    static int t=0;
    //double w=M_PI*167.00/120;
    double w=M_PI*0.5;
    double a=3.2;
    //double r=a/w/w;
    Bullet b(320, 240, 0, 0, BULLET("rice"), instance->scene);
    b.setAcceleration(a).setRotateSpeed(w);
    instance->scene->pushBullet(b);
    t++;
    return 0;
}

LuaTaskTimeline::LuaTaskTimeline(BulletScene* bulletScene)
    :scene(bulletScene)
    ,currentTask(0)
{
    instance=this;
}

LuaTaskTimeline::~LuaTaskTimeline()
{
    clear();
}

void LuaTaskTimeline::setTime(double newTime)
{
    timeSec=newTime;
}

void LuaTaskTimeline::push(const LuaTask& task)
{
    taskMap[task.luaRef]=task;
    taskList.push(&taskMap[task.luaRef]);
}

void LuaTaskTimeline::update(double deltaSec)
{
    double newSec=timeSec+deltaSec;

    int result=LUA_OK;

    while(true)
    {
        currentTask=taskList.top();

        if(currentTask->nextTime>newSec)
            break;

        taskList.pop();
        timeSec=currentTask->nextTime;

        result=LUA_OK;

        if(currentTask->luaThread)
            result=lua_resume(currentTask->luaThread, 0, 0);

        if(result!=LUA_YIELD)
        {
            luaL_unref(_L, LUA_REGISTRYINDEX, currentTask->luaRef);
            taskMap.erase(currentTask->luaRef);
            continue;
        }

        taskList.push(currentTask);

    }

    timeSec=newSec;

}

double LuaTaskTimeline::seekNextTask(double limitDeltaSec)
{
    if(taskList.empty())
    {
        return limitDeltaSec;
    }

    double newSec=timeSec+limitDeltaSec;
    LuaTask* task=taskList.top();

    if(task->nextTime>newSec)
    {
        timeSec=newSec;
        return 0.0;
    }

    timeSec=task->nextTime;
    return newSec-timeSec;

}

double LuaTaskTimeline::updateSingleTask(double limitDeltaSec)
{
    if(taskList.empty())
    {
        return limitDeltaSec;
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
        result=lua_resume(currentTask->luaThread, 0, 0);

    if(result!=LUA_YIELD)
    {
        luaL_unref(_L, LUA_REGISTRYINDEX, currentTask->luaRef);
        taskMap.erase(currentTask->luaRef);
    }
    else
    {
        taskList.push(currentTask);
    }

    return newSec-timeSec;
}

void LuaTaskTimeline::clear()
{
    while(!taskList.empty())
    {
        if(_L)
        {
            LuaTask* t=taskList.top();
            luaL_unref(_L, LUA_REGISTRYINDEX, t->luaRef);
        }
        taskList.pop();
    }
    taskMap.clear();
}

bool LuaTaskTimeline::empty()
{
    return taskList.empty();
}


