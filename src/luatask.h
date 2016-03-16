#ifndef LUATASK_H
#define LUATASK_H

#include "lua/lua.hpp"

#include "bullet.h"
#include "bulletscene.h"

#include <set>
#include <list>
using namespace std;

class SoundBuffer;

struct LuaTask
{
    lua_State* luaThread;
    int luaRef;
    double nextTime;

    bool isFinished;

    double centerX, centerY;
    double angle;
    int bulletType;

    SoundBuffer* sound;

    double deltaX, deltaY;
    int centerObj;

    //set<int> attachList;

    /*
     * todo:
     * no more todos yet.
     */

    LuaTask();
    LuaTask(lua_State* thread, int ref, double beginTime, const LuaTask* center=0);

    /*
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
    }*/

    inline void delay(double timeSec)
    {
        nextTime+=timeSec;
    }

    inline void delayUntil(double timeSec)
    {
        nextTime=timeSec;
    }

    inline void updateCenter(BulletScene* scene)
    {
        if(centerObj == 0)
        {
            centerX=deltaX;
            centerY=deltaY;
        }
        else
        {
            auto center = scene->getBullet(centerObj);
            if(center == nullptr || center->dead() || center->destroyed())
            {
                isFinished=true;
            }
            else
            {
                centerX=deltaX+center->getX();
                centerY=deltaY+center->getY();
            }
        }
    }

    friend class LuaTaskTimeline;

};

#endif // LUATASK_H



