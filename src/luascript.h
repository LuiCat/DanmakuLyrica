class LuaScript;

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include "lua/lua.hpp"

#define lua_tableregister(L,n,f) (lua_pushstring(L,n),lua_pushcfunction(L,f),lua_settable(L,-3))
#define _L (LuaScript::getLuaState())

class LuaScript
{
protected:

    static lua_State* luaState;

    static char currentPath[256];

    static int lua_setCurrentPath(lua_State* L);

public:

    static void registerLuaFuncs();

    static int lua_excall(int narg, int nres);
    static int lua_exload(const char* cstr);

    static lua_State* lua_createthread(int idx);

    static bool loadScriptFile(const char* filename);

    static void init();
    static void cleanup();

    static lua_State* getLuaState();

    static const char* getCurrentPath();

};

#endif
