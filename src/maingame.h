#ifndef MAINGAME_H
#define MAINGAME_H

#include "windef.h"

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

    virtual void mainInit();
    virtual void mainCleanup();
    virtual void mainUpdate();
    virtual void mainRender();

    void waitForUpdateTick();
    void updateFps(DWORD rate);

    void prepare();
    void finish();

public:

    explicit MainGame();

    static DWORD WINAPI mainThreadFunc(LPVOID);

    static void CALLBACK tickFrame(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

    void start(HWND hWnd);
    void exit();

    inline bool exiting() const
    {
        return isExiting;
    }

    void updateTick();
    void process();

};

#endif // MAINGAME_H
