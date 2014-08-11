#include "keybinding.h"

#include "commondef.h"
#include "debug.h"

LPDIRECTINPUT8       lpDirectInput = 0;

list<KeyBinding*> KeyBinding::bindingList(0);

unordered_map<DWORD, KeyBinding*> KeyBinding::keyboardMap;
unordered_map<DWORD, KeyBinding*> KeyBinding::joystickMap;

LPDIRECTINPUTDEVICE8 KeyBinding::lpKeyboard = 0;
LPDIRECTINPUTDEVICE8 KeyBinding::lpJoystick = 0;

double KeyBinding::stickTriggerRange = 0.5;
bool KeyBinding::useBufferedData = false;

HRESULT DInput_Init(HWND hWnd, HINSTANCE hInst)
{
    UNUSED(hInst);

    HRESULT hr;

    if(FAILED(hr=KeyBinding::initDevice(hWnd)))
        return hr;

    return S_OK;
}


void DInput_Cleanup()
{
    KeyBinding::cleanupDevice();
}


KeyBinding::KeyBinding(DWORD _keyKeyboard, DWORD _keyJoystick)
    :keyKeyboard(0)
    ,keyJoystick(0)
    ,pushed(false)
    ,down(false)
    ,value(0)
    ,callbackFunc(0)
{
    bindingList.push_back(this);
    setKey(_keyKeyboard, true);
    setKey(_keyJoystick, false);
}

KeyBinding::~KeyBinding()
{
    setKey(0, true);
    setKey(0, false);
    bindingList.remove(this);
}

HRESULT KeyBinding::initDevice(HWND hWnd)
{
    HRESULT hr;

    if(FAILED(hr=initKeyboard(hWnd)))
    {
        cout<<"Keyboard init failed "<<hex<<hr<<endl;
        return hr;
    }

    if(FAILED(hr=initJoystick(hWnd)))
    {
        cout<<"Joystick init failed "<<hex<<hr<<endl;
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
    refreshKeyboard();
    refreshJoystick();
}

bool KeyBinding::isPushed() const
{
    return pushed;
}

bool KeyBinding::isDown() const
{
    return down;
}

DWORD KeyBinding::getValue() const
{
    return value;
}

bool KeyBinding::operator()() const
{
    return isDown();
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
        map[newKey]=this;
    }

    oldKey=newKey;

    return true;
}

void KeyBinding::setCallbackFunc(BindingCallback func)
{
    callbackFunc=func;
}

HRESULT KeyBinding::initKeyboard(HWND hWnd)
{
    UNUSED(hWnd);
    return S_OK;
}

void KeyBinding::cleanupKeyboard()
{
}

void KeyBinding::refreshKeyboard()
{
    unordered_map<DWORD, KeyBinding*>::iterator iter;

    DWORD currentTick=GetTickCount();

    DWORD key;
    for(auto x : bindingList)
    {
        key=x->keyKeyboard;
        x->pushKeyData(true, key, GetAsyncKeyState(key), currentTick);
    }

    for(auto x : bindingList)
    {
        x->dealKeyData();
    }
}

HRESULT KeyBinding::initJoystick(HWND hWnd)
{
    UNUSED(hWnd);
    return S_OK;
}

void KeyBinding::cleanupJoystick()
{
}

void KeyBinding::refreshJoystick()
{
}

void KeyBinding::pushKeyData(bool isKeyboard, DWORD key, DWORD data, DWORD timeStamp)
{
    KeyData keyData={isKeyboard, key, data, timeStamp};
    dataBuffer.push_back(keyData);
}

void KeyBinding::dealKeyData()
{
    KeyData* keyData;

    bool newDown;

    value=0;
    pushed=false;

    while(!dataBuffer.empty())
    {
        keyData=&(dataBuffer.front());

        if(callbackFunc)
            callbackFunc(this, keyData);

        if(keyData->isKeyBoard)
        {
            newDown=(((SHORT)keyData->data)<0);
            if(newDown && !down)
            {
                pushed=true;
                value++;
            }
            down=newDown;
        }
        else
        {
            // ...
        }

        dataBuffer.pop_front();
    }
}
