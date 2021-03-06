#include "maingame.h"

#include "sounds.h"
#include "soundregistry.h"

#include "graphics.h"
#include "keybinding.h"

#include "commondef.h"
#include "mathhelper.h"

#include "debug.h"

MainGame* MainGame::instance=0;

MainGame::MainGame()
    :tick(0)
    ,hWnd(0)
    ,mainThread(0)
    ,hTickSema(0)
    ,idTimer(0)
    ,lastFrameTick(0)
    ,frameRate(0.0f)
    ,isExiting(false)
{
}

void MainGame::start(HWND hWnd)
{
    instance=this;

    this->hWnd=hWnd;

    mainThread=CreateThread(0, 0, mainThreadFunc, 0, 0, 0);
    CloseHandle(mainThread);
}

void MainGame::exit()
{
    isExiting=true;
}

void MainGame::updateTick()
{
    ReleaseSemaphore(hTickSema, 1, 0);
}

void MainGame::waitForUpdateTick()
{
    ++tick;
#ifndef FPS_UNLIMITED
    WaitForSingleObject(hTickSema, INFINITE);
#endif
}

void MainGame::updateFps(DWORD rate)
{
    if(lastFrameTick==0)
    {
        lastFrameTick=GetTickCount();
        return;
    }
    char buffer[60];
    DWORD newFrameTick=GetTickCount();
    frameRate=0.0;
    if(newFrameTick>lastFrameTick)
        frameRate=rate*1000.0/(newFrameTick-lastFrameTick);
    lastFrameTick=newFrameTick;
    sprintf_s(buffer, 59, "Danmaku Lyrica [Fps%.1f]", frameRate);
    SetWindowText(hWnd, buffer);
}

void MainGame::prepare()
{
    hTickSema=CreateSemaphore(NULL, 0, MAX_UNUPDATED_FRAME, "tickupdate");
#ifndef FPS_UNLIMITED
    idTimer=timeSetEvent(FPS_LIMIT_MSEC, 1, tickFrame, 0, TIME_PERIODIC);
#endif
    lastFrameTick=0;
    tick=0;
}

void MainGame::finish()
{
    instance=0;
    CloseHandle(hTickSema);
    lastFrameTick=0;
    PostMessage(hWnd, WM_QUIT, 0, 0);
}

void MainGame::process()
{
    KeyBinding::refreshAll();

    mainUpdate();

    mainRender();

    if(tick%60==0)
        updateFps(60);

    waitForUpdateTick();
}

DWORD MainGame::mainThreadFunc(LPVOID)
{
    instance->prepare();

    instance->mainInit();

    while(!instance->isExiting)
    {
        instance->process();
    }

    instance->mainCleanup();

    instance->finish();

    return 0;
}

void CALLBACK MainGame::tickFrame(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
    if(!instance)
        return;
    instance->updateTick();
}
