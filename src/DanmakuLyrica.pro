TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_LFLAGS += -static-libgcc
QMAKE_LFLAGS += -static-libstdc++


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
    maingame.cpp

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
    maingame.h

LIBS += -ld3d9 \
		-ld3dx9 \
		-lwinmm \
		-ldsound \
		-ldxguid

