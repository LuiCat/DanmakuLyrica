#ifndef LUATASK_H
#define LUATASK_H

#include "lua/lua.hpp"

#include "bullet.h"

#include <list>
using namespace std;

class LuaTask
{
public:

    lua_State* luaThread;
    int luaRef;
    double nextTime;

    double centerX, centerY;
    double angle;

    list<Bullet*> attachList;

    /*
     * todo:
     * bullet type, sound type, etc.
     */

    LuaTask();
    LuaTask(lua_State* thread, int ref, double beginTime);

    inline void attachBullet(Bullet* b)
    {
        attachList.push_back(b);
    }

    inline void clearAttach()
    {
        attachList.clear();
    }


    friend class LuaTaskTimeline;

};

#endif // LUATASK_H



