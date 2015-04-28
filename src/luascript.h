class LuaScript;

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include "lua/lua.hpp"

#define lua_tableregister(L,n,f) (lua_pushstring(L,n),lua_pushcfunction(L,f),lua_settable(L,-3))

typedef int (*LuaFunc)(lua_State*);

class LuaScript
{
private:

    static int lua_setCurrentPath(lua_State* L);

protected:

    static LuaScript* currentInstance;

    lua_State* luaState;
    char currentPath[256];

    void beginScript()
    {
        currentInstance=this;
    }

    void endScript()
    {
        currentInstance=0;
    }

public:

    LuaScript();
    ~LuaScript();

    int lua_excall(int narg, int nres);
    int lua_exload(const char* cstr);

    lua_State* lua_createthread(int idx);

    bool loadScriptFile(const char* filename);

    void registerFunc(const char* name, LuaFunc func);
    void registerTableFunc(const char* name, LuaFunc func);

    inline lua_State* L() const
    {
        return luaState;
    }

    inline operator lua_State* () const
    {
        return luaState;
    }

    inline const char* getCurrentPath()
    {
        return currentPath;
    }

};

#endif
