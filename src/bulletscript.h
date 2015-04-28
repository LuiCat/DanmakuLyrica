#ifndef LUABULLETSCRIPT_H
#define LUABULLETSCRIPT_H

#include "luatimeline.h"
#include "bulletattachlist.h"

class BulletScript : public LuaTimeline
{
private:

    inline static BulletScript* inst()
    {
        return (BulletScript*)currentInstance;
    }

    void registerLuaFuncs();

    static int lua_registerBullet(lua_State* L);
    static int lua_pushBullet(lua_State* L);
    static int lua_setCenter(lua_State* L);
    static int lua_setAngle(lua_State* L);
    static int lua_setBulletType(lua_State* L);

    static int lua_attachAlloc(lua_State* L);
    static int lua_attachBullet(lua_State* L);
    static int lua_clearAttach(lua_State* L);

    static int lua_setAttachedBulletPosition(lua_State* L);
    static int lua_setAttachedBulletRotation(lua_State* L);
    static int lua_setAttachedBulletSpeed(lua_State* L);
    static int lua_setAttachedBulletAcceleration(lua_State* L);
    static int lua_setAttachedBulletRotateSpeed(lua_State* L);
    static int lua_setAttachedBulletPositionOffset(lua_State* L);
    static int lua_setAttachedBulletRotationOffset(lua_State* L);
    static int lua_setAttachedBulletSpeedOffset(lua_State* L);

protected:

    BulletScene* scene;
    BulletList* bulletList;

public:

    explicit BulletScript(BulletScene* scene);

};

#endif // LUABULLETSCRIPT_H
