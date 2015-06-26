#include "animation.h"

Animation::Animation()
{

}

Animation::~Animation()
{

}

void Animation::clear()
{
    frames.clear();
}

bool Animation::fromFile(const char*)
{
    // to do: animation file format
    return false;
}

void Animation::setLoop(bool doLoop)
{
    loop=doLoop;
}

void Animation::pushFrame(Texture texture, double x, double y, double width, double height)
{
    frames.emplace_back(texture, x, y, width, height);
}

void Animation::pushVertices(int i)
{
    int c=frames.size();
    if(c<=0)
    {
        d3d.setTexture(0);
        d3d.vertex(0.0, 0.0, 0.0, 0.0);
        d3d.vertex(1.0, 0.0, 1.0, 0.0);
        d3d.vertex(1.0, 1.0, 1.0, 1.0);
        d3d.vertex(0.0, 1.0, 0.0, 1.0);
    }
    else
    {
        if(i<0)i=-i;
        i%=c;
        Frame& f=frames[i];

        d3d.setTexture(f.tex);

        double umax=f.x+f.w;
        double vmax=f.y+f.h;

        d3d.vertex(0.0, 0.0, f.x,  f.y );
        d3d.vertex(1.0, 0.0, umax, f.y );
        d3d.vertex(1.0, 1.0, umax, vmax);
        d3d.vertex(0.0, 1.0, f.x,  vmax);
    }
}

