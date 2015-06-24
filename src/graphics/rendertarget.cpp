#include "rendertarget.h"


list<D3DResource*> D3DResource::resList;

HRESULT D3DResource::lostAll()
{
    list<D3DResource*>::iterator iter=resList.end();
    while(iter!=resList.begin())
    {
        --iter;
        if(FAILED((*iter)->onLost()))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT D3DResource::resetAll()
{
    list<D3DResource*>::iterator iter=resList.begin();
    while(iter!=resList.end())
    {
        if(FAILED((*iter)->onReset()))
            return E_FAIL;
        ++iter;
    }
    return S_OK;
}

D3DResource::D3DResource()
{
    resList.push_back(this);
}

D3DResource::~D3DResource()
{
    resList.remove(this);
}

HRESULT D3DResource::onLost()
{
    return S_OK;
}

HRESULT D3DResource::onReset()
{
    return S_OK;
}
