#ifndef RENDERSURFACE_H
#define RENDERSURFACE_H

#include "rendertarget.h"
#include <stack>
using namespace std;

class RenderSurface : public RenderTarget
{
private:

    Surface pD3DSurface;

protected:

    Surface surface();

public:

    RenderSurface();
    RenderSurface(int width, int height);
    ~RenderSurface();

};

#endif // RENDERSURFACE_H
