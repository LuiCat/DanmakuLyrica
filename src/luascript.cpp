#include "luascript.h"

#include <algorithm>

#include "mathhelper.h"

#include "debug.h"

//===========================================================================

LuaScript* LuaScript::currentInstance = 0;

int LuaScript::lua_setCurrentPath(lua_State* L)
{
    if(lua_isstring(L, 1))
        currentInstance->currentPath = lua_tostring(L, 1);
    return 0;
}

//===========================================================================

int LuaScript::lua_excall(int narg, int nres)
{
    beginScript();
    int res=lua_pcall(luaState, narg, nres, 0);
    endScript();
    if(res!=0)cout<<"[CALL ERROR]"<<lua_tostring(luaState, -1)<<", error code : "<<res<<endl;
	return res;
}

int LuaScript::lua_exload(const char* cstr)
{
    beginScript();
    int res=luaL_loadfile(luaState, cstr);
    endScript();
    if(res!=0)cout<<"[LOAD ERROR]"<<lua_tostring(luaState, -1)<<", error code : "<<res<<endl;
    return res;
}

lua_State* LuaScript::lua_createthread(int idx)
{
    if(!lua_isfunction(luaState, idx))return 0;
    lua_pushvalue(luaState, idx);
    lua_State* T=lua_newthread(luaState);
    lua_pop(luaState, 1);
    lua_xmove(luaState, T, 1);
    return T;
}

bool LuaScript::loadScriptFile(const char* filename)
{
    if(lua_exload(filename)==0)
    {
        currentPath = filename;
        replace(currentPath.begin(), currentPath.end(), '\\', '/');
        auto found = currentPath.rfind('/');
        if (found != string::npos)
            currentPath.resize(found + 1);
        else
            currentPath = "/";
        return (lua_excall(0, 0)==LUA_OK);
    }
    return false;
}

//===========================================================================

LuaScript::LuaScript()
{
    luaState=luaL_newstate();
    luaL_openlibs(luaState);
    registerFunc("setCurrentPath", lua_setCurrentPath);
}

LuaScript::~LuaScript()
{
    if(luaState)
        lua_close(luaState);
    luaState=0;
}

void LuaScript::registerFunc(const char* name, LuaFunc func)
{
    lua_register(luaState, name, func);
}

void LuaScript::registerTableFunc(const char* name, LuaFunc func)
{
    lua_tableregister(luaState, name, func);
}


