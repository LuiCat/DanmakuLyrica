#include "bulletscript.h"

#include "luatask.h"

#include "debug.h"
#include "functionevent.h"
#include "movement.h"
#include "soundregistry.h"

void BulletScript::registerLuaFuncs()
{
    registerFuncName(loadStream);
    registerFuncName(loadSound);
    registerFuncName(playSound);
    registerFuncName(playSoundPitch);

    registerFuncName(registerBullet);
    registerFuncName(pushBullet);
    registerFuncName(setCenter);
    registerFuncName(setAngle);
    registerFuncName(setBulletType);
    registerFuncName(setSound);

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

int BulletScript::lua_loadStream(lua_State* L)
{
    if(!lua_isstring(L, 1))
        return 0;

    int buflen = strlen(inst()->getCurrentPath())+strlen(lua_tostring(L, 1));
    char* buffer = new char[buflen+1];
    strcpy(buffer, inst()->getCurrentPath());
    strcat(buffer, lua_tostring(L, 1));

    double vol=(lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 1.0);
    const char* name=(lua_isstring(L, 2) ? lua_tostring(L, 2) : 0);

    int id=SoundRegistry::createSound<StreamBuffer>(name, buffer, vol);
    lua_pushinteger(L, id);

    return 1;
}

int BulletScript::lua_loadSound(lua_State* L)
{
    if(!lua_isstring(L, 1))
        return 0;

    int buflen = strlen(inst()->getCurrentPath())+strlen(lua_tostring(L, 1));
    char* buffer = new char[buflen+1];
    strcpy(buffer, inst()->getCurrentPath());
    strcat(buffer, lua_tostring(L, 1));

    double vol=(lua_isnumber(L, -1) ? lua_tonumber(L, -1) : 1.0);
    const char* name=(lua_isstring(L, 2) ? lua_tostring(L, 2) : 0);

    int id=SoundRegistry::createSound<SoundBuffer>(name, buffer, vol);
    lua_pushinteger(L, id);

    return 1;
}

int BulletScript::lua_playSound(lua_State* L)
{
    SoundBuffer* sound=0;
    if(lua_gettop(L)==0)
    {
        sound=inst()->currentTask->sound;
    }
    else if(lua_isstring(L, 1))
    {
        sound=SoundRegistry::get(lua_tostring(L, 1));
    }
    else if(lua_isnumber(L, 1))
    {
        sound=SoundRegistry::get(lua_tonumber(L, 1));
    }

    if(sound)
    {
        sound->resetPitch();
        sound->play(true);
    }
    return 0;
}

int BulletScript::lua_playSoundPitch(lua_State* L)
{
    if(!lua_isnumber(L, -1))
        return 0;

    SoundBuffer* sound=0;
    int top = lua_gettop(L);
    if(top==1)
    {
        sound=inst()->currentTask->sound;
    }
    else if(top==2)
    {
        if(lua_isstring(L, 1))
        {
            sound=SoundRegistry::get(lua_tostring(L, 1));
        }
        else if(lua_isnumber(L, 1))
        {
            sound=SoundRegistry::get(lua_tonumber(L, 1));
        }
    }

    if(sound)
    {
        sound->setPitch(lua_tonumber(L, -1));
        sound->play(true);
    }
    return 0;
}

int BulletScript::lua_registerBullet(lua_State* L)
{
    if(lua_gettop(L)!=10)return 0;

    if(!lua_isstring(L, 1))return 0;

    BulletType type;
    const char *str;

    int buflen = strlen(inst()->getCurrentPath())+strlen(lua_tostring(L, 2));
    char* buffer = new char[buflen+1];
    strcpy(buffer, inst()->getCurrentPath());
    strcat(buffer, lua_tostring(L, 2));

    type.image.load(buffer);

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

    lua_pushinteger(L, Bullet::reg(lua_tostring(L, 1), type));

    cout<<"Bullet \""<<lua_tostring(L, 1)<<"\" Registered, texture id "<<type.image.texture()<<endl;

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

    if(task->sound)
        task->sound->play(true);

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

int BulletScript::lua_setSound(lua_State* L)
{
    LuaTask* task=inst()->currentTask;
    if(lua_gettop(L) == 0)
    {
        task->sound=0;
        return 0;
    }
    SoundBuffer* sound=0;
    if(lua_isnumber(L, 1))
    {
        sound=SoundRegistry::get(lua_tointeger(L, 1));
    }
    else if(lua_isstring(L, 1))
    {
        sound=SoundRegistry::get(lua_tostring(L, 1));
    }
    if(sound)
    {
        task->sound=sound;
    }
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
    if(lua_isnumber(L, 4))
    {
        double delay=lua_tonumber(L, 4);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::posx, false, lua_tonumber(L, 2));
            });
        if(lua_isnumber(L, 3))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::posy, false, lua_tonumber(L, 2));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setX, lua_tonumber(L, 2));
        if(lua_isnumber(L, 3))
            p->forEach(BulletBase::setY, lua_tonumber(L, 3));
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletRotation(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 3))
    {
        double delay=lua_tonumber(L, 3);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::angle, false, rad(lua_tonumber(L, 2)));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setRotation, rad(lua_tonumber(L, 2)));
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletSpeed(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 3))
    {
        double delay=lua_tonumber(L, 3);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::speed, false, rad(lua_tonumber(L, 2)));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setSpeed, lua_tonumber(L, 2));
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletAcceleration(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 3))
    {
        double delay=lua_tonumber(L, 3);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::acceleration, false, lua_tonumber(L, 2));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setAccelerateSpeed, lua_tonumber(L, 2));
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletRotateSpeed(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 3))
    {
        double delay=lua_tonumber(L, 3);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::rotatespeed, false, rad(lua_tonumber(L, 2)));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setRotateSpeed, rad(lua_tonumber(L, 2)));
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletPositionOffset(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 4))
    {
        double delay=lua_tonumber(L, 4);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::posx, true, lua_tonumber(L, 2));
            });
        if(lua_isnumber(L, 3))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::posy, true, lua_tonumber(L, 2));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b){b.setX(b.getX()+lua_tonumber(L, 2));});
        if(lua_isnumber(L, 3))
            p->forEach([&](BulletBase& b){b.setY(b.getY()+lua_tonumber(L, 3));});
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletRotationOffset(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 3))
    {
        double delay=lua_tonumber(L, 3);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::angle, true, rad(lua_tonumber(L, 2)));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setRotationOffset, rad(lua_tonumber(L, 2)));
    }
    return 0;
}

int BulletScript::lua_setAttachedBulletSpeedOffset(lua_State* L)
{
    BulletAttachList* p=READYLIST;
    if(lua_isnumber(L, 3))
    {
        double delay=lua_tonumber(L, 3);
        if(lua_isnumber(L, 2))
            p->forEach([&](BulletBase& b)
            {
                b.pushEvent<Movement>(delay, Movement::speed, true, lua_tonumber(L, 2));
            });
    }
    else
    {
        if(lua_isnumber(L, 2))
            p->forEach(BulletBase::setSpeedOffset, lua_tonumber(L, 2));
    }
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

