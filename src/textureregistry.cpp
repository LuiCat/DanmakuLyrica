#include "textureregistry.h"

unordered_map<string, Texture> TextureRegistry::reg;

Texture TextureRegistry::newTexture(const char* filename)
{
    string str(filename);
    unordered_map<string, Texture>::iterator iter=reg.find(str);
    if(iter==reg.end())
    {
        Texture tex=0;
        createTexture(filename, &tex);
        reg.emplace(str, tex);
        return tex;
    }
    return iter->second;
}

void TextureRegistry::registerTexture(const char* filename, Texture tex)
{
    reg[string(filename)]=tex;
}
