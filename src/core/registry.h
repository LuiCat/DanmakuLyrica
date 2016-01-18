#ifndef REGISTRY_H
#define REGISTRY_H

#include <unordered_map>
#include <string>
#include <cstring>

using namespace std;

template<typename T>
class Registry
{
private:

    unordered_map<int, T> typeInfoList;
    unordered_map<string, int> typeNameMap;

    int lastRegisteredId;

public:

    Registry()
        :lastRegisteredId(0)
    {

    }

    int registerName(const char* name, const T& info)
    {
        ++lastRegisteredId;
        if(name && strlen(name)>0)
            typeNameMap[string(name)]=lastRegisteredId;
        typeInfoList.emplace(lastRegisteredId, std::move(info));
        return lastRegisteredId;
    }

    inline int operator()(const char* name, const T& info)
    {
        return registerName(name, info);
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
        return &typeInfoList[id];
    }

    template <typename Function>
    void releaseAll(Function func)
    {
        while(!typeInfoList.empty())
        {
            func(&(typeInfoList.begin()->second));
            typeInfoList.erase(typeInfoList.begin());
        }
    }

};

#endif // REGISTRY_H
