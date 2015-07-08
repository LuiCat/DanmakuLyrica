#include "resimage.h"
#include "graphics.h"

ResImage::ResImage(const string& filePath)
    :Resource(filePath)
{
    createTexture(filePath.c_str(), &texture);
}

ResImage::~ResImage()
{

}

