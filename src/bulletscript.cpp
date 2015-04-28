#include "bulletscript.h"

#include "luatask.h"

#include "debug.h"

void BulletScript::registerLuaFuncs()
{
    registerFunc("registerBullet", lua_registerBullet);
    registerFunc("pushBullet", lua_pushBullet);
    registerFunc("setCenter", lua_setCenter);
    registerFunc("setAngle", lua_setAngle);
    registerFunc("setBulletType", lua_setBulletType);

    lua_createtable(luaState, 0, 8);
    registerTableFunc("alloc", lua_attachAlloc);
    registerTableFunc("attach", lua_attachBullet);
    registerTableFunc("clear", lua_clearAttach);
    registerTableFunc("setPos", lua_setAttachedBulletPosition);
    registerTableFunc("setRot", lua_setAttachedBulletRotation);
    registerTableFunc("setSpeed", lua_setAttachedBulletSpeed);
    registerTableFunc("setAcc", lua_setAttachedBulletAcceleration);
    registerTableFunc("setRS", lua_setAttachedBulletRotateSpeed);
    registerTableFunc("offsetPos", lua_setAttachedBulletPositionOffset);
    registerTableFunc("offsetRot", lua_setAttachedBulletRotationOffset);
    registerTableFunc("offsetSpeed", lua_setAttachedBulletSpeedOffset);
    lua_setglobal(luaState, "Attach");
}

int BulletScript::lua_registerBullet(lua_State* L)
{
    if(lua_gettop(L)!=10)return 0;

    if(!lua_isstring(L, 1))return 0;

    BulletType type;
    char buffer[256];
    const char *str;

    strcpy(buffer, inst()->getCurrentPath());
    strcat(buffer, lua_tostring(L, 2));
    type.texture=0;
    DWORD r;
    if((r=createTexture(buffer, &type.texture))!=D3D_OK)
        return 0;

    type.sizeX=lua_tonumber(L, 3);
    type.sizeY=lua_tonumber(L, 4);
    type.centerX=lua_tonumber(L, 5);
    type.centerY=lua_tonumber(L, 6);

    if(lua_isstring(L, 7))
    {
        str=lua_tostring(L, 7);
        if(strcmp(str, "spin")==0||strcmp(str, "Spin")==0)
        {
            type.facingType=BulletType::spin;
        }
        else if(strcmp(str, "forward")==0||strcmp(str, "Forward")==0)
        {
            type.facingType=BulletType::forward;
        }
        else
        {
            type.facingType=BulletType::none;
        }
    }
    else
    {
        type.facingType=(BulletType::FacingType)lua_tointeger(L, 7);
    }

    if(lua_isstring(L, 7))
    {
        str=lua_tostring(L, 7);
        if(strcmp(str, "oval")==0||strcmp(str, "Oval")==0)
        {
            type.judgeType=BulletType::oval;
        }
        else if(strcmp(str, "rect")==0||strcmp(str, "Rect")==0)
        {
            type.judgeType=BulletType::rect;
        }
        else
        {
            type.judgeType=BulletType::round;
        }
    }
    else
    {
        type.judgeType=(BulletType::JudgeType)lua_tointeger(L, 8);
    }

    type.judgeSizeX=lua_tonumber(L, 9);
    type.judgeSizeY=lua_tonumber(L, 10);

    lua_pushinteger(L, Bullet::reg(lua_tostring(L, 1), &type));

    cout<<"Bullet \""<<lua_tostring(L, 1)<<"\" Registered, texture id "<<type.texture<<endl;

    return 1;
}

int BulletScript::lua_pushBullet(lua_State* L)
{
    if(lua_gettop(L)<4)
        return 0;

    LuaTask* task=inst()->currentTask;
    double x=lua_tonumber(L, 1)+task->centerX;
    double y=lua_tonumber(L, 2)+task->centerY;
    double sp=lua_tonumber(L, 3);
    double rt=rad(lua_tonumber(L, 4))+task->angle;
    double type=(lua_isstring(L, 5)?BULLET(lua_tostring(L, 5)):lua_isnumber(L, 5)?lua_tointeger(L, 5):task->bulletType);

    lua_pushinteger(L, inst()->scene->pushBullet(x, y, sp, rt, type));

    return 1;
}

int BulletScript::lua_setCenter(lua_State* L)
{
    LuaTask* task=inst()->currentTask;
    if(lua_isnumber(L, 1))
        task->centerX=lua_tonumber(L, 1);
    if(lua_isnumber(L, 2))
        task->centerY=lua_tonumber(L, 2);
    return 0;
}

int BulletScript::lua_setAngle(lua_State* L)
{
    LuaTask* task=inst()->currentTask;
    if(lua_isnumber(L, 1))
        task->angle=lua_tonumber(L, 1);
    return 0;
}

int BulletScript::lua_setBulletType(lua_State* L)
{
    LuaTask* task=inst()->currentTask;
    if(lua_isnumber(L, 1))
        task->bulletType=lua_tointeger(L, 1);
    else if(lua_isstring(L, 1))
        task->bulletType=BULLET(lua_tostring(L, 1));
    return 0;
}

int BulletScript::lua_attachAlloc(lua_State* L)
{
    BulletAttachList* p=(BulletAttachList*)lua_newuserdata(L, sizeof(BulletAttachList));
    p=new(p) BulletAttachList(inst()->bulletList);
    return 1;
}

#define READYLIST (lua_pushstring(L, "list"),lua_gettable(L, 1),(BulletAttachList*)lua_touserdata(L, -1))

int BulletScript::lua_attachBullet(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    int t=lua_gettop(L);
    for(int i=2;i<t;++i)
    {
        if(lua_isnumber(L, i))
        {
            p->attach(lua_tointeger(L, i));
        }
    }
    return 0;
}

int BulletScript::lua_clearAttach(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    p->clear();
    return 0;
}

int BulletScript::lua_setAttachedBulletPosition(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setX, lua_tonumber(L, 2));
    if(lua_isnumber(L, 3))
        p->forEach(BulletBase::setY, lua_tonumber(L, 3));
    return 0;
}

int BulletScript::lua_setAttachedBulletRotation(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setRotation, rad(lua_tonumber(L, 2)));
    return 0;
}

int BulletScript::lua_setAttachedBulletSpeed(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setSpeed, lua_tonumber(L, 2));
    return 0;
}

int BulletScript::lua_setAttachedBulletAcceleration(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setAcceleration, lua_tonumber(L, 2));
    return 0;
}

int BulletScript::lua_setAttachedBulletRotateSpeed(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setRotateSpeed, rad(lua_tonumber(L, 2)));
    return 0;
}

int BulletScript::lua_setAttachedBulletPositionOffset(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    int t;
    if(lua_isnumber(L, 2))
    {
        t=lua_tonumber(L, 2);
        p->forEach([t](BulletBase& b){b.setX(b.getX()+t);});
    }
    if(lua_isnumber(L, 3))
    {
        t=lua_tonumber(L, 3);
        p->forEach([t](BulletBase& b){b.setY(b.getY()+t);});
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletRotationOffset(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setRotationOffset, rad(lua_tonumber(L, 2)));
    return 0;
}

int BulletScript::lua_setAttachedBulletSpeedOffset(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 2))
        p->forEach(BulletBase::setSpeedOffset, lua_tonumber(L, 2));
    return 0;
}

//==================================================================

BulletScript::BulletScript(BulletScene* scene)
    :scene(scene)
{
    bulletList=(scene?scene->getBulletList():0);

    registerLuaFuncs();

    loadScriptFile("data/script/utils.lua");
    loadScriptFile("data/script/attach.lua");
    loadScriptFile("data/script/reg_bullet.lua");

}
