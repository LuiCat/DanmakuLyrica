#ifndef KEYBINDING_H
#define KEYBINDING_H

#include <windows.h>

#include <dinput.h>

#include <unordered_map>
#include <list>
using namespace std;

#define DINPUT_BUFFERSIZE 50
#define KEYDATA_BUFFERSIZE 20

struct KeyInfo
{
    DWORD data;
    DWORD timeStamp;
};

class KeyBinding
{
public:

    static HRESULT initDevice(HWND hWnd);
    static void cleanupDevice();

    static void acquire();
    static void unacquire();

    static void refreshAll();

    KeyBinding(DWORD _keyKeyboard=0, DWORD _keyJoystick=0);
    ~KeyBinding();

    void refresh();

    virtual bool status();
    virtual int value();

    bool operator()();

    bool setKey(DWORD key, bool isKeyboard);

protected:

    static list<KeyBinding*> bindingList;
    static unordered_map<DWORD, KeyBinding*> keyboardMap;
    static unordered_map<DWORD, KeyBinding*> joystickMap;

    static LPDIRECTINPUTDEVICE8 lpKeyboard;
    static LPDIRECTINPUTDEVICE8 lpJoystick;

    static HRESULT initKeyboard(HWND hWnd);
    static void cleanupKeyboard();

    static HRESULT initJoystick(HWND hWnd);
    static void cleanupJoystick();

    virtual void pushKeyData(DWORD data);

private:

    DWORD keyKeyboard;
    DWORD keyJoystick;

    bool pushed;
    int pushCount;

    list<KeyInfo> dataBuffer;

};

HRESULT DInput_Init(HWND hWnd, HINSTANCE hInst);
void DInput_Cleanup();

#endif // KEYBINDING_H
