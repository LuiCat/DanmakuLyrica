#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include "entity.h"

#include <map>
#include <list>
using namespace std;

template <typename T>
class EntityList
{
    static_assert(std::is_base_of<Entity, T>::value, "Template parameter is not an Entity based class");

public:

    EntityList()
        :nextEntityID(0)
        ,autoClear(true)
    {
        clearAll();
    }

    int pushEntity(const T& entity)
    {
        entityList[nextEntityID]=entity;
        return nextEntityID++;
    }

    void pushEntities(const list<T>& entities)
    {
        for(const T& e : entities)
        {
            pushEntity(e);
        }
    }

    template <class... Args>
    int newEntity(Args&&... args)
    {
        entityList.emplace_hint(entityList.end(), piecewise_construct, make_tuple(nextEntityID), make_tuple(args...));
        return nextEntityID++;
    }

    void updateAll(double deltaSec)
    {
        for(pair<const int, T>& x : entityList)
        {
            x.second.update(deltaSec);
        }
        if(autoClear)
            clearDead();
    }

    void renderAll()
    {
        for(pair<const int, T>& x : entityList)
        {
            x.second.render();
        }
    }

    void clearAll()
    {
        entityList.clear();
    }

    void clearDead()
    {
        auto iter=entityList.begin();
        while(iter!=entityList.end())
        {
            if(iter->second.dead())
            {
                iter=entityList.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    void setDeadAutoClear(bool clear)
    {
        autoClear=clear;
    }

    int size() const
    {
        return entityList.size();
    }

protected:

    map<int, T> entityList;

private:

    int nextEntityID;

    bool autoClear;

};

#endif // ENTITYLIST_H
