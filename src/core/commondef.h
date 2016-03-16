#ifndef COMMONDEF_H
#define COMMONDEF_H

#include <windows.h>
#include <cassert>

#define WINDOWED

#define WIDTH  800
#define HEIGHT 600

#define WM_RESETDEVICE (WM_APP|0x00)

#define FPS_UNLIMITED
//#define FPS_VSYNC
#define FPS_LIMIT_MSEC 6
#define MAX_UNUPDATED_FRAME 5

#define UNUSED(x) (void)(x)

#define LYRICA_ASSERT(x) assert(x)

#define SAFE_RELEASE(p) {if(p){delete p;p=0;}}

#define SINGLE_INST(T) \
private:\
static unique_ptr<T> m_inst;

#define MAKE_RGB(r,g,b) (0xFF000000|(((r)<<16)&0xFF0000)|(((g)<<8)&0xFF00)|((b)&0xFF))
#define MAKE_RGBA(r,g,b,a) ((((a)<<24)&0xFF000000)|(((r)<<16)&0xFF0000)|(((g)<<8)&0xFF00)|((b)&0xFF))

#endif // COMMONDEF_H
