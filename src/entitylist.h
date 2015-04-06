#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include "entity.h"

#include <list>
using namespace std;

template <typename T>
class EntityList
{
    static_assert(std::is_base_of<Entity, T>::value, "Template parameter is not an Entity based class");

public:

    EntityList()
        :autoClear(true)
    {
        clearAll();
    }

    void pushEntity(const T& entity)
    {
        entityList.push_back(entity);
    }

    void pushEntities(list<T>& entities)
    {
        for(T& e : entities)
        {
            pushEntity(&e);
        }
    }

    template <class... Args>
    void newEntity(Args&&... args)
    {
        entityList.emplace_back(args...);
    }

    void updateAll(double deltaSec)
    {
        for(T& x : entityList)
        {
            x.update(deltaSec);
        }
        if(autoClear)
            clearDead();
    }

    void renderAll()
    {
        for(T& x : entityList)
        {
            x.render();
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

    void setDeadAutoClear(bool clear)
    {
        autoClear=clear;
    }

    int size() const
    {
        return entityList.size();
    }

protected:

    list<T> entityList;

private:

    bool autoClear;

};

#endif // ENTITYLIST_H
