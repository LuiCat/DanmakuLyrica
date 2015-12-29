#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "gfxcore.h"
#include "gfxdef.h"

#include <unordered_map>
#include <string>
using namespace std;

class TextureCache
{
private:

    static unordered_map<string, Texture> cache;

public:

    static bool isCached(const char* filename);

    static Texture load(const char* filename);
    static Texture loadFromCache(const char* filename);

};

#endif // TEXTURECACHE_H
