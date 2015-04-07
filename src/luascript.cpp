#include "luascript.h"

#include <cstring>

#include "mathhelper.h"

#include "debug.h"

//===========================================================================

lua_State* LuaScript::luaState = 0;
char LuaScript::currentPath[256] = "/";

//===========================================================================

int LuaScript::lua_excall(int narg, int nres)
{
    int res=lua_pcall(luaState, narg, nres, 0);
    if(res!=0)cout<<"[CALL ERROR]"<<lua_tostring(luaState, -1)<<", error code : "<<res<<endl;
	return res;
}

int LuaScript::lua_exload(const char* cstr)
{
    int res=luaL_loadfile(luaState, cstr);
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

bool LuaScript::loadScriptFile(const char* filename){
    if(lua_exload(filename)==0)
    {
        strcpy(currentPath, filename);
        for(int i=strlen(currentPath)-1;i>=0&&currentPath[i]!='/';--i)
            currentPath[i]=0;
        return (lua_excall(0, 0)==LUA_OK);
    }
    return false;
}

//===========================================================================


void LuaScript::initLuaFuncs()
{

}

void LuaScript::init()
{
    luaState=luaL_newstate();

    luaL_openlibs(luaState);

    initLuaFuncs();
}

void LuaScript::cleanup()
{
    if(luaState)
        lua_close(luaState);
    luaState=0;
}

lua_State* LuaScript::getLuaState()
{
    return luaState;
}

const char* LuaScript::getCurrentPath()
{
    return currentPath;
}
