#include "entitylist.h"

EntityList::EntityList()
    :autoClear(true)
{
    clearAll();
}

void EntityList::updateAll(double deltaSec)
{
    for(auto x : entityList)
    {
        x->update(deltaSec);
    }
    if(autoClear)
        clearDead();
}

void EntityList::renderAll()
{
    for(auto x : entityList)
    {
        x->render();
    }
}

void EntityList::clearAll()
{
    while(!entityList.empty())
    {
        delete entityList.front();
        entityList.pop_front();
    }
}

void EntityList::clearDead()
{
    auto iter=entityList.begin();
    while(iter!=entityList.end())
    {
        if((*iter)->dead())
        {
            delete *iter;
            iter=entityList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void EntityList::setDeadAutoClear(bool clear)
{
    autoClear=clear;
}
