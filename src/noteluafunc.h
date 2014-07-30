#ifndef NOTELUAFUNC_H
#define NOTELUAFUNC_H

#include "luascript.h"

class NoteLuaFunc
{
public:
    static void registerLuaFuncs(lua_State *L);
protected:
    int lua_sleepBeat(lua_State *L);
};

#endif // NOTELUAFUNC_H
