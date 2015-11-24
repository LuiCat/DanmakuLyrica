#ifndef FUNCTIONEVENT_H
#define FUNCTIONEVENT_H

#include "entityevent.h"

#include <functional>
using namespace std;

class FunctionEvent : public EntityEvent
{
public:

    template <typename Function>
    FunctionEvent(double processTime, Function processFunc)
        :EntityEvent(processTime)
        ,func(processFunc)
    {

    }

protected:

    void onProcess(Entity* entity);

private:

    function<void(Entity*)> func;

};

#endif // FUNCTIONEVENT_H
