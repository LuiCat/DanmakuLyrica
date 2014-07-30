#include "noteluafunc.h"



void NoteLuaFunc::registerLuaFuncs(lua_State *L)
{
    lua_register(L, "sleepBeat", lua_sleepBeat);
}


int NoteLuaFunc::lua_sleepBeat(lua_State *L)
{
    return 0;
}
