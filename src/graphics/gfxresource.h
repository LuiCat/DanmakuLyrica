#ifndef GFXRESOURCE_H
#define GFXRESOURCE_H

#include "gfxdef.h"
#include "gfxcore.h"
#include <list>
using namespace std;

class GFXResource : public GFXCore
{
public:

    static HRESULT lostAll();
    static HRESULT resetAll();

    GFXResource();
    virtual ~GFXResource()=0;

protected:

    virtual HRESULT lost();
    virtual HRESULT reset();

private:

    static list<GFXResource*> resList;

};

#endif // GFXRESOURCE_H
