#ifndef ANIMATEDTEXTURE_H
#define ANIMATEDTEXTURE_H

#include "graphics.h"

#include <vector>
using namespace std;

class AnimatedTexture
{
private:

    struct Frame
    {
        Texture tex;
        double x,y,w,h;
        Frame(Texture texture, double x, double y, double width, double height)
            :tex(texture)
            ,x(x)
            ,y(y)
            ,w(width)
            ,h(height)
        {

        }
    };

    vector<Frame> frames;

    bool loop;

protected:

public:

    AnimatedTexture();
    ~AnimatedTexture();

    void clear();
    bool fromFile(const char* url);

    void setLoop(bool doLoop);
    void pushFrame(Texture texture, double x, double y, double width, double height);

    void pushVertices(int i);



};

#endif // ANIMATEDTEXTURE_H
