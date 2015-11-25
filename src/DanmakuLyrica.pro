TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++

QMAKE_CFLAGS_RELEASE += -O2

DEFINES -= UNICODE

SOURCES += main.cpp \
    graphics/gfxcore.cpp \
    graphics/textrenderer.cpp \
    graphics/vertexbuffer.cpp \
    graphics/gfxresource.cpp \
    graphics/rendersurface.cpp \
    graphics/rendertarget.cpp \
    graphics/rendertexture.cpp \
    sound/soundregistry.cpp \
    sound/sounds.cpp \
    keybinding/keybinding.cpp \
    core/registry.cpp \
    core/timeline.cpp \
    core/ticking.cpp \
    core/beatticking.cpp \
    entity/entity.cpp \
    entity/position.cpp \
    entity/entitylist.cpp \
    entity/entityattachlist.cpp \
    entity/entityevent.cpp \
    entity/movement.cpp \
    entity/functionevent.cpp \
    scene/scene.cpp \
    scene/scenemanager.cpp \
    animatedtexture.cpp \
    maingame.cpp \
    danmakulyrica.cpp \
    luascript.cpp \
    segment.cpp \
    noteentity.cpp \
    notescene.cpp \
    notelist.cpp \
    notejudge.cpp \
    mapstate.cpp \
    texturestrip.cpp \
    notemap.cpp \
    bullet.cpp \
    bulletscene.cpp \
    luatask.cpp \
    bulletbase.cpp \
    bulletlist.cpp \
    bulletattachlist.cpp \
    luatimeline.cpp \
    bulletscript.cpp \
    textureregistry.cpp \
    entity/renderobject.cpp \
    entity/sprite.cpp

HEADERS += \
    graphics/graphics.h \
    graphics/gfxcore.h \
    graphics/gfxdef.h \
    graphics/textrenderer.h \
    graphics/vertexbuffer.h \
    graphics/gfxresource.h \
    graphics/rendersurface.h \
    graphics/rendertarget.h \
    graphics/rendertexture.h \
    sound/soundregistry.h \
    sound/sounds.h \
    keybinding/keybinding.h \
    core/debug.h \
    core/commondef.h \
    core/mathhelper.h \
    core/registry.h \
    core/ticking.h \
    core/beatticking.h \
    entity/entity.h \
    entity/position.h \
    entity/entitylist.h \
    entity/entityattachlist.h \
    entity/entityevent.h \
    entity/movement.h \
    entity/functionevent.h \
    scene/scene.h \
    scene/scenemanager.h \
    maingame.h \
    danmakulyrica.h \
    luascript.h \
    segment.h \
    notedef.h \
    noteentity.h \
    notescene.h \
    notelist.h \
    notejudge.h \
    mapstate.h \
    timeline.h \
    texturestrip.h \
    notemap.h \
    bullet.h \
    bulletscene.h \
    luatask.h \
    bulletdef.h \
    bulletbase.h \
    bulletlist.h \
    bulletattachlist.h \
    luatimeline.h \
    bulletscript.h \
    textureregistry.h \
    animatedtexture.h \
    bullettype.h \
    entity/renderobject.h \
    entity/sprite.h

LIBS += -ld3d9 \
        -ld3dx9 \
        -lwinmm \
        -ldsound \
        -ldinput8 \
        -ldxguid

LIBS += $$PWD/lua/lua.a \
        $$PWD/libVorbisFile.a \
        $$PWD/libVorbis.a \
        $$PWD/libOgg.a

DISTFILES += \
    todo.txt

INCLUDEPATH += \
    graphics \
    sound \
    keybinding \
    core \
    entity \
    scene \
    script
