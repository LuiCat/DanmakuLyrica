#ifndef KEYBINDING_H
#define KEYBINDING_H

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <unordered_map>
#include <list>
using namespace std;

#define DINPUT_BUFFERSIZE 500

struct KeyData
{
    bool isKeyBoard;
    DWORD key;
    DWORD data;
    DWORD timeStamp;
};

class KeyBinding;

typedef void(*BindingCallback)(KeyBinding*, const KeyData*);

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

    bool isPushed() const;
    bool isDown() const;
    DWORD getValue() const;

    bool operator()() const;

    bool setKey(DWORD key, bool isKeyboard);

    void setCallbackFunc(BindingCallback func);

protected:

    static list<KeyBinding*> bindingList;
    static unordered_map<DWORD, KeyBinding*> keyboardMap;
    static unordered_map<DWORD, KeyBinding*> joystickMap;

    static LPDIRECTINPUTDEVICE8 lpKeyboard;
    static LPDIRECTINPUTDEVICE8 lpJoystick;

    static double stickTriggerRange;
    static bool useBufferedData;

    static HRESULT initKeyboard(HWND hWnd);
    static void cleanupKeyboard();
    static void refreshKeyboard();

    static HRESULT initJoystick(HWND hWnd);
    static void cleanupJoystick();
    static void refreshJoystick();

    static void dealAllKeyData();

    void pushKeyData(bool isKeyboard, DWORD key, DWORD data, DWORD timeStamp);
    void dealKeyData();

private:

    DWORD keyKeyboard;
    DWORD keyJoystick;

    bool pushed;
    bool down;
    DWORD value;

    list<KeyData> dataBuffer;

    BindingCallback callbackFunc;

};

HRESULT DInput_Init(HWND hWnd, HINSTANCE hInst);
void DInput_Cleanup();

#endif // KEYBINDING_H
