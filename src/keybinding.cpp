#include "keybinding.h"

#include "commondef.h"
#include "debug.h"

LPDIRECTINPUT8       lpDirectInput = 0;

list<KeyBinding*> KeyBinding::bindingList(0);

unordered_map<DWORD, KeyBinding*> KeyBinding::keyboardMap;
unordered_map<DWORD, KeyBinding*> KeyBinding::joystickMap;

LPDIRECTINPUTDEVICE8 KeyBinding::lpKeyboard = 0;
LPDIRECTINPUTDEVICE8 KeyBinding::lpJoystick = 0;

HRESULT DInput_Init(HWND hWnd, HINSTANCE hInst)
{
    HRESULT hr;

    if(FAILED(hr=DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&lpDirectInput, NULL)))
        return hr;

    if(FAILED(hr=KeyBinding::initDevice(hWnd)))
        return hr;

    return S_OK;
}


void DInput_Cleanup()
{
    if (lpDirectInput)
    {
        lpDirectInput->Release();
        lpDirectInput=0;
    }
}


KeyBinding::KeyBinding(DWORD _keyKeyboard, DWORD _keyJoystick)
{
    bindingList.push_back(this);
    setKey(_keyKeyboard, true);
    setKey(_keyJoystick, true);
}

KeyBinding::~KeyBinding()
{
    setKey(0, true);
    setKey(0, true);
    bindingList.remove(this);
}

HRESULT KeyBinding::initDevice(HWND hWnd)
{
    HRESULT hr;

    if(FAILED(hr=initKeyboard(hWnd)))
    {
        cout<<"Keyboard init failed "<<hr<<endl;
        return hr;
    }

    if(FAILED(hr=initJoystick(hWnd)))
    {
        cout<<"Joystick init failed "<<hr<<endl;
    }

    return S_OK;
}

void KeyBinding::cleanupDevice()
{
    cleanupKeyboard();
    cleanupJoystick();
}

void KeyBinding::acquire()
{
    lpKeyboard->Acquire();
    lpJoystick->Acquire();
}

void KeyBinding::unacquire()
{
    lpKeyboard->Unacquire();
    lpJoystick->Unacquire();
}

void KeyBinding::refreshAll()
{
    // ...
    for(auto x : bindingList)
        x->refresh();
}

void KeyBinding::refresh()
{
    // ...
}

bool KeyBinding::status()
{
    return pushed;
}

int KeyBinding::value()
{
    return pushCount;
}

bool KeyBinding::operator()()
{
    return status();
}

bool KeyBinding::setKey(DWORD newKey, bool isKeyboard)
{
    unordered_map<DWORD, KeyBinding*>::iterator iter;
    unordered_map<DWORD, KeyBinding*>& map=(isKeyboard?keyboardMap:joystickMap);
    DWORD& oldKey=(isKeyboard?keyKeyboard:keyJoystick);

    if(newKey)
    {
        iter=map.find(newKey);
        if(iter!=map.end())
            return false;
    }

    if(oldKey)
    {
        iter=map.find(oldKey);
        if(iter!=map.end() && iter->second==this)
            map.erase(iter);
    }

    if(newKey)
    {
        map[(newKey)]=this;
    }

    oldKey=newKey;

    return true;
}

HRESULT KeyBinding::initKeyboard(HWND hWnd)
{
    HRESULT hr;

    if(FAILED(hr=lpDirectInput->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL)))
        return hr;

    if(FAILED(hr=lpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
        return hr;

    if(FAILED(hr=lpKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
        return hr;

    DIPROPDWORD     property;

    property.diph.dwSize = sizeof(DIPROPDWORD);
    property.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    property.diph.dwObj = 0;
    property.diph.dwHow = DIPH_DEVICE;
    property.dwData = DINPUT_BUFFERSIZE;

    if(FAILED(hr=lpKeyboard->SetProperty(DIPROP_BUFFERSIZE, &property.diph)))
        return hr;

    if(FAILED(hr=lpKeyboard->Acquire()))
        return hr;

    return S_OK;
}

void KeyBinding::cleanupKeyboard()
{
    if(lpKeyboard)
    {
        lpKeyboard->Unacquire();
        lpKeyboard->Release();
        lpKeyboard=0;
    }
}

HRESULT KeyBinding::initJoystick(HWND hWnd)
{
    UNUSED(hWnd);
    return S_OK;
}

void KeyBinding::cleanupJoystick()
{
    if(lpJoystick)
    {
        lpJoystick->Unacquire();
        lpJoystick->Release();
        lpJoystick=0;
    }
}

void KeyBinding::pushKeyData(DWORD data)
{
    UNUSED(data);
    // ...
}
