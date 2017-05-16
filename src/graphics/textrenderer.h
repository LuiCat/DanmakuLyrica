#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "gfxcore.h"
#include "gfxdef.h"
#include "gfxresource.h"

#include <sstream>
using namespace std;

class TextRenderer : public GFXResource
{
private:

    LPD3DXFONT lpfont;

    int height;
    int width;
    int weight;
    bool italic;
    //char facename[128];

    stringstream ss;

protected:

    HRESULT lost();
    HRESULT reset();

public:

    TextRenderer(int height, int width, int weight, bool italic, const char* font);

    /// Use this like whatever you do to std::cout
    template <typename T>
    TextRenderer& operator<<(T x)
    {
        ss<<x;
        return *this;
    }

    void clear();

    /// Render the text
    void present(UINT format, D3DCOLOR color=0x000000);

};

#endif // TEXTRENDERER_H
