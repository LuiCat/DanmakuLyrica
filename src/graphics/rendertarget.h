#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include "gfxresource.h"

class RenderTarget : public GFXResource
{
private:

    Surface lastSurface;

    bool isTarget;

protected:

    int width;
    int height;

public:

    RenderTarget(int width, int height);

    virtual Surface surface()=0;

    void clear();

    void setTarget();
    void unsetTarget();

    HRESULT present(int x, int y, RenderTarget* target=0);

};

#endif // RENDERTARGET_H
