TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++

QMAKE_CFLAGS_RELEASE += -O2

DEFINES -= UNICODE

SOURCES += main.cpp \
    graphics.cpp \
    entity.cpp \
	object.cpp \
    position.cpp \
    registry.cpp \
    soundregistry.cpp \
    sounds.cpp \
    ticking.cpp \
    maingame.cpp \
    danmakulyrica.cpp \
    luascript.cpp \
    entitylist.cpp \
    notemap.cpp \
    segment.cpp \
    noteluafunc.cpp \
    noteentity.cpp \
    notescene.cpp \
    notelist.cpp \
    notejudge.cpp \
    mapstate.cpp \
    timeline.cpp \
    keybinding.cpp \
    texturestrip.cpp

HEADERS += \
    debug.h \
    graphics.h \
    entity.h \
    mathhelper.h \
    position.h \
    registry.h \
    soundregistry.h \
    sounds.h \
    ticking.h \
    object.h \
    maingame.h \
    danmakulyrica.h \
    luascript.h \
    entitylist.h \
    notemap.h \
    segment.h \
    notedef.h \
    commondef.h \
    noteluafunc.h \
    noteentity.h \
    notescene.h \
    notelist.h \
    notejudge.h \
    mapstate.h \
    timeline.h \
    keybinding.h \
    texturestrip.h

LIBS += -ld3d9 \
		-ld3dx9 \
		-lwinmm \
		-ldsound \
		-ldinput8 \
		-ldxguid

LIBS += $$PWD/lua/lua.a
