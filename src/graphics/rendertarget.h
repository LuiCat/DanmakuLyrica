#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "gfxdef.h"
#include <list>
using namespace std;

class D3DResource
{
public:

    static HRESULT lostAll();
    static HRESULT resetAll();

    D3DResource();
    virtual ~D3DResource()=0;

protected:

    virtual HRESULT onLost();
    virtual HRESULT onReset();

private:

    static list<D3DResource*> resList;

};

#endif // RENDERTARGET_H
