#ifndef LUATASK_H
#define LUATASK_H

#include "lua/lua.hpp"

#include "bullet.h"

#include <set>
#include <list>
using namespace std;

struct LuaTask
{
    lua_State* luaThread;
    int luaRef;
    double nextTime;

    double centerX, centerY;
    double angle;
    int bulletType;

    set<int> attachList;

    /*
     * todo:
     * bullet type, sound type, etc.
     */

    LuaTask();
    LuaTask(lua_State* thread, int ref, double beginTime, const LuaTask* center=0);

    inline void attachBullet(int bulletID)
    {
        attachList.insert(bulletID);
    }

    inline void attachBullets(const list<int>& listID)
    {
        for(int bulletID : listID)
        {
            attachList.insert(bulletID);
        }
    }

    inline void clearAttach()
    {
        attachList.clear();
    }

    friend class LuaTaskTimeline;

};

#endif // LUATASK_H



