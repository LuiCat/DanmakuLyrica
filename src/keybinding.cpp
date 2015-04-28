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
bool KeyBinding::useBufferedData = true;


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
    KeyBinding::cleanupDevice();

    if (lpDirectInput)
    {
        lpDirectInput->Release();
        lpDirectInput=0;
    }
}

KeyBinding::KeyBinding(DWORD _keyKeyboard, DWORD _keyJoystick)
    :pushed(false)
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
    dealAllKeyData();
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

void KeyBinding::refreshKeyboard()
{
    if(!lpKeyboard)
        return;

    HRESULT hr = DI_OK;

    BYTE                diks[256];
    DIDEVICEOBJECTDATA  didod[DINPUT_BUFFERSIZE];
    DWORD               dwElements = DINPUT_BUFFERSIZE;

    unordered_map<DWORD, KeyBinding*>::iterator iter;

    DWORD currentTick=GetTickCount();

    hr=lpKeyboard->Acquire();

    hr=DI_OK;

    if(useBufferedData)
    {
        hr=lpKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0);
        if(hr==DI_OK || hr==DI_BUFFEROVERFLOW)
        {
            for(DWORD i=0; i<dwElements; ++i)
            {
                iter=keyboardMap.find(didod[i].dwOfs);
                if(iter==keyboardMap.end())
                    continue;
                iter->second->pushKeyData(true, didod[i].dwOfs, didod[i].dwData, didod[i].dwTimeStamp);
            }
        }
    }

    if(!useBufferedData || hr==DI_BUFFEROVERFLOW)
    {
        hr=lpKeyboard->GetDeviceState(256, diks);
        if(hr==DI_OK)
        {
            DWORD key;
            for(auto x : bindingList)
            {
                key=x->keyKeyboard;
                x->pushKeyData(true, key, diks[key], currentTick);
            }
        }
    }
}

HRESULT KeyBinding::initJoystick(HWND hWnd)
{
    UNUSED(hWnd);
    // ...
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

void KeyBinding::refreshJoystick()
{
    // ...
}

void KeyBinding::dealAllKeyData()
{
    for(auto x : bindingList)
    {
        x->dealKeyData();
    }
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
            newDown=(((keyData->data)&0x80)!=0);
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
