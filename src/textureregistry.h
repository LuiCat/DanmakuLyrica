#ifndef TEXTUREREGISTRY_H
#define TEXTUREREGISTRY_H

#include "registry.h"
#include "graphics.h"

#include <string>
#include <unordered_map>
using namespace std;

class TextureRegistry
{
private:
    static unordered_map<string, Texture> reg;
public:
    static Texture newTexture(const char* filename);
    static void registerTexture(const char* filename, Texture tex);
};

#endif // TEXTUREREGISTRY_H
