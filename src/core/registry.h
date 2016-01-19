#ifndef REGISTRY_H
#define REGISTRY_H

#include <unordered_map>
#include <string>
#include <cstring>
#include <memory>

using namespace std;

template<typename T>
class Registry
{
private:

    unordered_map<int, unique_ptr<T>> typeInfoList;
    unordered_map<string, int> typeNameMap;

    int lastRegisteredId;

public:

    Registry()
        :lastRegisteredId(0)
    {

    }

    template <typename _T>
    int registerName(const char* name, const _T& info)
    {
        ++lastRegisteredId;
        if(name && strlen(name)>0)
            typeNameMap[string(name)]=lastRegisteredId;
        typeInfoList.emplace(lastRegisteredId, unique_ptr<_T>(new _T(info)));
        return lastRegisteredId;
    }

    template <typename _T>
    int registerName(const char* name, _T&& info)
    {
        ++lastRegisteredId;
        if(name && strlen(name)>0)
            typeNameMap[string(name)]=lastRegisteredId;
        typeInfoList.emplace(lastRegisteredId,
              unique_ptr<T>(new _T(std::forward<_T>(info))));
        return lastRegisteredId;
    }

    template <typename _T, typename... Args>
    int registerName(const char* name, Args&&... args)
    {
        ++lastRegisteredId;
        if(name && strlen(name)>0)
            typeNameMap[string(name)]=lastRegisteredId;
        typeInfoList.emplace(lastRegisteredId, unique_ptr<_T>(new _T(args...)));
        return lastRegisteredId;
    }

    template <typename... Args>
    inline int operator()(const char* name, Args&&... args)
    {
        return registerName(name, std::forward<Args>(args)...);
    }

    inline int getId(const char* typeName)
    {
        string str(typeName);
        if(typeNameMap.find(str)==typeNameMap.end())return 0;
        return typeNameMap[str];
    }

    inline T* getInfo(int id)
    {
        if(typeInfoList.find(id)==typeInfoList.end())
            return 0;
        return typeInfoList[id].get();
    }

    template <typename Function>
    void releaseAll(Function func)
    {
        for(auto& pair : typeInfoList)
        {
            func(pair.second.get());
        }
        typeInfoList.clear();
        typeNameMap.clear();
    }

};

#endif // REGISTRY_H
