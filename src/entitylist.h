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

    void pushEntity(T* entity)
    {
        T* p=dynamic_cast<T*>(entity);
        if(p)
            entityList.push_back(p);
    }

    void pushEntities(list<T>& entities)
    {
        for(T& e : entities)
        {
            pushEntity(&e);
        }
    }

    void pushEntities(list<T*>& entities)
    {
        for(T* e : entities)
        {
            pushEntity(e);
        }
    }

    void updateAll(double deltaSec)
    {
        for(auto x : entityList)
        {
            x->update(deltaSec);
        }
        if(autoClear)
            clearDead();
    }

    void renderAll()
    {
        for(auto x : entityList)
        {
            x->render();
        }
    }

    void clearAll()
    {
        while(!entityList.empty())
        {
            delete entityList.front();
            entityList.pop_front();
        }
    }

    void clearDead()
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

    void setDeadAutoClear(bool clear)
    {
        autoClear=clear;
    }

    int size() const
    {
        return entityList.size();
    }

protected:

    list<Entity*> entityList;

private:

    bool autoClear;

};

#endif // ENTITYLIST_H
