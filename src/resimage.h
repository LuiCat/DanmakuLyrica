#ifndef RESIMAGE_H
#define RESIMAGE_H

#include "resource.h"
#include "graphics/gfxdef.h"

#define TEXTURE(s) ((Texture)Resource::get<ResImage>(s))

class ResImage : public Resource
{
private:

    Texture texture;

protected:

    explicit ResImage(const string& filePath);
    ~ResImage();

    friend class Resource;

public:

    inline operator Texture() const
    {
        return texture;
    }

};

#endif // RESIMAGE_H
