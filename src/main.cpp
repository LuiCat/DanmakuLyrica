#include <windows.h>
#include <windowsx.h>

#include "graphics.h"
#include "sounds.h"
#include "keybinding.h"

#include "soundregistry.h"
#include "luascript.h"

#include "danmakulyrica.h"

#include "commondef.h"
#include "debug.h"


HWND       hWnd       = 0;
HINSTANCE  hInstance  = 0;

DanmakuLyrica *game;

void Main_Init()
{
    if(FAILED(D3D_Init(hWnd)))throw "Can't init d3d";
    if(FAILED(Sound_Init(hWnd)))throw "Can't init sound";
    if(FAILED(DInput_Init(hWnd, hInstance)))throw "Can't init dinput";
}

void Main_Cleanup()
{
    SoundRegistry::releaseAll();
    DInput_Cleanup();
    Sound_Cleanup();
    D3D_Cleanup();
}

LRESULT WINAPI msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CLOSE:
        if(game)
            game->exit();
        else
            PostQuitMessage(0);
        game=0;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if(wParam==VK_ESCAPE)
        {
            if(game)
                game->exit();
            game=0;
        }
        return 0;
    case WM_RESETDEVICE:
        d3d.beginScene();
        if(FAILED(d3d.reset()))
        {
            throw "Can't reset d3d device, device has lost";
        }
        d3d.endScene();
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


WINAPI INT WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    hInstance=hInst;

    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, msgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        "Danmaku Lyrica", NULL
    };
    RegisterClassEx(&wc);

    #ifndef WINDOWED
    hWnd = CreateWindowEx(WS_EX_LAYERED|WS_EX_TOPMOST, "Danmaku Lyrica", "Danmaku Lyrica",
                              WS_POPUP, 0, 0, WIDTH, HEIGHT,
                              NULL, NULL, wc.hInstance, NULL);
    #else
    hWnd = CreateWindow("Danmaku Lyrica", "Danmaku Lyrica",
                              WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX, 0, 0, WIDTH, HEIGHT,
                              NULL, NULL, wc.hInstance, NULL);
    #endif

    #ifdef WINDOWED
    RECT winrect={0, 0, WIDTH, HEIGHT};
    AdjustWindowRectEx(&winrect,GetWindowStyle(hWnd),GetMenu(hWnd)!=NULL,GetWindowExStyle(hWnd));
    MoveWindow(hWnd,297,100,winrect.right-winrect.left,winrect.bottom-winrect.top,false);
    #endif

    #ifndef WINDOWED
    ShowCursor(false);
    #endif

    try
    {
        Main_Init();

        game=new DanmakuLyrica();
        game->start(hWnd);

        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        MSG msg;
        ZeroMemory(&msg, sizeof(msg));

        while(GetMessage(&msg, hWnd, 0, 0)>0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    catch(const char* str)
    {
        cout<<"[ERROR]"<<str<<endl;
        MessageBox(hWnd, str, 0, MB_OK|MB_ICONERROR);
    }

    Main_Cleanup();

    UnregisterClass("Danmaku Lyrica", wc.hInstance);

    return 0;
}
