class LuaScript;

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <lua.hpp>
#include <string>

#define lua_tableregister(L,n,f) (lua_pushstring(L,n),lua_pushcfunction(L,f),lua_settable(L,-3))

#define registerFuncName(n) LuaScript::registerFunc(#n,lua_##n)

typedef int (*LuaFunc)(lua_State*);

class LuaScript
{
private:

    static int lua_setCurrentPath(lua_State* L);

protected:

    static LuaScript* currentInstance;

    lua_State* luaState;
    std::string currentPath;

    // Used before and after lua_excall.
    // Sets current instance so lua state can be accessed in registered static functions
    void beginScript();
    void endScript();

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

    inline const std::string& getCurrentPath()
    {
        return currentPath;
    }

};

#endif
