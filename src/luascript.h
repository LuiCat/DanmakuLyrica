class LuaScript;

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include "lua/lua.hpp"

#define lua_tableregister(L,n,f) (lua_pushstring(L,n),lua_pushcfunction(L,f),lua_settable(L,-3))

class LuaScript
{
private:

    static lua_State* luaState;

    static void initLuaFuncs();

protected:

    static int lua_excall(int narg, int nres);
    static int lua_exload(const char* cstr);

    static lua_State* lua_createthread(int idx);

    static bool loadScriptFile(const char* filename);

public:

    static void init();

    static void cleanup();

};

#endif