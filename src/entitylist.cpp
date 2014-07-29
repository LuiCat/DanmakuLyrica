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
        x.update(deltaSec);
    }
    if(autoClear)
        clearDead();
}

void EntityList::renderAll()
{
    for(auto x : entityList)
    {
        x.render();
    }
}

void EntityList::clearAll()
{
    entityList.clear();
}

void EntityList::clearDead()
{
    auto iter=entityList.begin();
    while(iter!=entityList.end())
    {
        if(iter->dead())
        {
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
