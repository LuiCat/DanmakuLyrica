#ifndef LUATIMELINE_H
#define LUATIMELINE_H

#include "luatask.h"
#include "bulletscene.h"

#include "luascript.h"

#include "mathhelper.h"

#include <queue>
#include <map>
using namespace std;

class LuaTimeline : public LuaScript
{    
private:

    inline static LuaTimeline* inst()
    {
        return (LuaTimeline*)currentInstance;
    }

    void registerLuaFuncs();

    static int lua_pushTask(lua_State* L);
    static int lua_taskDelay(lua_State* L);
    static int lua_taskDelayUntil(lua_State* L);
    static int lua_getTime(lua_State* L);

    class LTComp
    {
    public:
        bool operator()(const LuaTask* a, const LuaTask* b)
        {
            return a->nextTime>b->nextTime;
        }
    };

    map<int, LuaTask> taskMap;
    priority_queue<LuaTask*, vector<LuaTask*>, LTComp> taskList;

protected:

    double timeSec;
    LuaTask* currentTask;

    virtual bool beforeTask(LuaTask* task);

public:

    LuaTimeline();
    ~LuaTimeline();

    void setTime(double newTime);

    void pushTask(const LuaTask& task);
    void clearTask();

    bool taskListEmpty();

    void update(double deltaSec);
    double seekNextTask(double limitDeltaSec=M_DINF);
    double updateSingleTask(double limitDeltaSec=M_DINF);

};


#endif // LUATIMELINE_H
