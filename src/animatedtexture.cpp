#include "animatedtexture.h"

AnimatedTexture::AnimatedTexture()
{

}

AnimatedTexture::~AnimatedTexture()
{

}

void AnimatedTexture::clear()
{
    frames.clear();
}

bool AnimatedTexture::fromFile(const char*)
{
    // to do: animation file format
    return false;
}

void AnimatedTexture::setLoop(bool doLoop)
{
    loop=doLoop;
}

void AnimatedTexture::pushFrame(Texture texture, double x, double y, double width, double height)
{
    frames.emplace_back(texture, x, y, width, height);
}

void AnimatedTexture::pushVertices(int i)
{
    int c=frames.size();
    if(c<=0)
    {
        d3d.setTexture(0);
        d3d.rect(0, 0, 1, 1);
    }
    else
    {
        if(i<0)i=-i;
        i%=c;
        Frame& f=frames[i];
        d3d.setTexture(f.tex);
        d3d.rect(f.x, f.y, f.x + f.w, f.y + f.h);
    }
}

