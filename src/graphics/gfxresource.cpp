#include "gfxresource.h"

list<GFXResource*> GFXResource::resList;

HRESULT GFXResource::lostAll()
{
    list<GFXResource*>::iterator iter=resList.end();
    while(iter!=resList.begin())
    {
        --iter;
        if(FAILED((*iter)->lost()))
            return E_FAIL;
    }
    return S_OK;
}

HRESULT GFXResource::resetAll()
{
    list<GFXResource*>::iterator iter=resList.begin();
    while(iter!=resList.end())
    {
        if(FAILED((*iter)->reset()))
            return E_FAIL;
        ++iter;
    }
    return S_OK;
}

GFXResource::GFXResource()
{
    resList.push_back(this);
}

HRESULT GFXResource::lost()
{
    return S_OK;
}

HRESULT GFXResource::reset()
{
    return S_OK;
}

GFXResource::~GFXResource()
{
    resList.remove(this);
}
