#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include "entity.h"

#include <map>
#include <set>
#include <list>
#include <functional>
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

    ~EntityList()
    {
        clearAll();
    }

    template <typename _T>
    int pushEntity(const _T& entity)
    {
        entityList[nextEntityID]=new _T(entity);
        return nextEntityID++;
    }

    template <typename _T>
    void pushEntities(const list<_T>& entities)
    {
        for(const _T& e : entities)
        {
            pushEntity(e);
        }
    }

    template <typename _T ,typename... Args>
    int newEntity(Args&&... args)
    {
        entityList.emplace_hint(entityList.end(), nextEntityID, new _T(args...));
        return nextEntityID++;
    }

    inline T* getEntity(int id)
    {
        return entityList[id];
    }

    inline T* operator[](int id)
    {
        return entityList[id];
    }

    inline bool hasEntity(int id)
    {
        return entityList.find(id)!=entityList.end();
    }

    template <typename Function>
    void forEach(Function fn)
    {
        for(pair<const int, T*>& x : entityList)
        {
            fn(*(x.second));
        }
    }

    template <typename Function>
    void forEach(const set<int>& listID, Function fn)
    {
        for(int id : listID)
        {
            auto iter=entityList.find(id);
            if(iter!=entityList.end())
                fn(*(iter->second));
        }
    }

    template <typename Function>
    void forEach(const list<int>& listID, Function fn)
    {
        for(int id : listID)
        {
            auto iter=entityList.find(id);
            if(iter!=entityList.end())
                fn(*(iter->second));
        }
    }

    template <typename MemberFunction, typename... Args>
    void forEach(const set<int>& listID, MemberFunction T::*fn, Args... args)
    {
        for(int id : listID)
        {
            auto iter=entityList.find(id);
            if(iter!=entityList.end())
                iter->second->*fn(args...);
        }
    }

    template <typename MemberFunction, typename... Args>
    void forEach(const list<int>& listID, MemberFunction T::*fn, Args... args)
    {
        for(int id : listID)
        {
            auto iter=entityList.find(id);
            if(iter!=entityList.end())
                iter->second->*fn(args...);
        }
    }

    void updateAll(double deltaSec)
    {
        for(pair<const int, T*>& x : entityList)
        {
            x.second->update(deltaSec);
        }
        if(autoClear)
            clearDead();
    }

    void renderAll()
    {
        for(pair<const int, T*>& x : entityList)
        {
            x.second->render();
        }
    }

    void clearAll()
    {
        auto iter=entityList.begin();
        while(iter!=entityList.end())
        {
            delete (iter->second);
            iter=entityList.erase(iter);
        }
        entityList.clear();
    }

    void clearDead()
    {
        auto iter=entityList.begin();
        while(iter!=entityList.end())
        {
            if(iter->second->dead())
            {
                delete (iter->second);
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

    map<int, T*> entityList;

private:

    int nextEntityID;

    bool autoClear;

};

#endif // ENTITYLIST_H
