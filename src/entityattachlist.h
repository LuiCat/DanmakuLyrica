#ifndef ENTITYATTACHLIST_H
#define ENTITYATTACHLIST_H

#include <set>
using namespace std;

#include "entitylist.h"
#include "entity.h"

template <typename T>
class EntityAttachList
{
protected:

    EntityList<T>* container;
    set<int> attachList;

public:

    EntityAttachList(EntityList<T>* list)
        :container(list)
    {

    }

    inline void attach(int entityID)
    {
        if(container)
            attachList.insert(entityID);
    }

    inline void attach(const list<int>& listID)
    {
        if(!container)return;
        for(int entityID : listID)
        {
            attachList.insert(entityID);
        }
    }

    inline void clear()
    {
        attachList.clear();
    }

    template <typename Function>
    void forEach(Function func)
    {
        container->forEach(attachList, func);
    }

    template <typename MemberFunction, typename... Args>
    void forEach(MemberFunction T::*func, Args... args)
    {
        container->forEach(attachList, func, args...);
    }

};

#endif // ENTITYATTACHLIST_H
