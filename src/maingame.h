#ifndef MAINGAME_H
#define MAINGAME_H

#include "windef.h"

#define FPS_UNLIMITED
#define MAX_UNUPDATED_FRAME 5

class MainGame
{
protected:

    static MainGame* instance;

    DWORD tick;

    HWND hWnd;

    HANDLE mainThread;

    HANDLE hTickSema;
    int idTimer;
    DWORD lastFrameTick;
    float frameRate;

    bool isExiting;

    void mainInit();
    void mainCleanup();
    void mainUpdate();
    void mainRender();

    void waitForUpdateTick();
    void updateFps(DWORD rate);

    void prepare();
    void finish();

public:

    MainGame();

    static DWORD WINAPI mainThreadFunc(LPVOID);

    static void CALLBACK tickFrame(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

    void start(HWND hWnd);
    void exit();

    void updateTick();
    void process();

};

#endif // MAINGAME_H
