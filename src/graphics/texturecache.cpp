#include "texturecache.h"

using namespace std;

unordered_map<string, Texture> TextureCache::cache;


bool TextureCache::isCached(const char* filename)
{
    return cache.count(filename)!=0;
}

Texture TextureCache::load(const char* filename)
{
    Texture tex = cache[filename];
    if(tex!=0)
        return tex;
    if(FAILED(createTexture(filename, &tex)))
        return 0;
    cache[filename] = tex;
    return tex;
}

Texture TextureCache::loadFromCache(const char* filename)
{
    return cache[filename];
}
