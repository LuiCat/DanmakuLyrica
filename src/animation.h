#ifndef ANIMATION_H
#define ANIMATION_H

#include "graphics.h"

#include <vector>
using namespace std;

class Animation
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

    Animation();
    ~Animation();

    void clear();
    bool fromFile(const char* url);

    void setLoop(bool doLoop);
    void pushFrame(Texture texture, double x, double y, double width, double height);

    void pushVertices(int i);



};

#endif // ANIMATION_H
