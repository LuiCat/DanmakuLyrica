#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include "entity.h"

#include <map>
#include <set>
#include <list>
#include <functional>
#include <memory>
using namespace std;

template <typename T>
class EntityList
{
    static_assert(std::is_base_of<Entity, T>::value, "Template parameter is not an Entity based class");

public:

    EntityList()
        :nextEntityID(1)
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
        entityList[nextEntityID].reset(new _T(entity));
        return nextEntityID++;
    }

    template <typename _T>
    int pushEntity(_T&& entity)
    {
        entityList[nextEntityID].reset(new _T(std::forward<_T>(entity)));
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
        entityList.emplace_hint(entityList.end(), nextEntityID, unique_ptr<_T>(new _T(args...)));
        return nextEntityID++;
    }

    inline T* getEntity(int id)
    {
        auto iter = entityList.find(id);
        if(iter == entityList.end())
            return nullptr;
        return iter->second.get();
    }

    inline T* operator[](int id)
    {
        auto iter = entityList.find(id);
        if(iter == entityList.end())
            return nullptr;
        return iter->second.get();
    }

    inline bool hasEntity(int id)
    {
        return entityList.find(id)!=entityList.end();
    }

    template <typename Function>
    void forEach(Function fn)
    {
        for(auto& x : entityList)
        {
            fn(*(x.second));
        }
    }

    template <typename MemberFunction, typename _T, typename... Args>
    void forEach(MemberFunction _T::*fn, Args... args)
    {
        for(auto& x : entityList)
        {
            (x.second.get()->*fn)(args...);
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

    template <typename MemberFunction, typename _T, typename... Args>
    void forEach(const set<int>& listID, MemberFunction _T::*fn, Args... args)
    {
        for(int id : listID)
        {
            auto iter=entityList.find(id);
            if(iter!=entityList.end())
                (iter->second.get()->*fn)(args...);
        }
    }

    template <typename MemberFunction, typename _T, typename... Args>
    void forEach(const list<int>& listID, MemberFunction _T::*fn, Args... args)
    {
        for(int id : listID)
        {
            auto iter=entityList.find(id);
            if(iter!=entityList.end())
                (iter->second.get()->*fn)(args...);
        }
    }

    void updateAll(double deltaSec)
    {
        for(auto& x : entityList)
        {
            x.second->update(deltaSec);
        }
        if(autoClear)
            clearDead();
    }

    void renderAll()
    {
        for(auto& x : entityList)
        {
            x.second->render();
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
            if(iter->second == nullptr || iter->second->dead())
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

    map<int, shared_ptr<T>> entityList;

private:

    int nextEntityID;

    bool autoClear;

};

#endif // ENTITYLIST_H
