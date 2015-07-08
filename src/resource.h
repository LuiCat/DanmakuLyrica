class Resource;

#ifndef RESOURCE_H
#define RESOURCE_H

#include <unordered_map>
#include <string>
using namespace std;

class Resource
{
public:

    enum ResourceType
    {
        unknown, image, sound, bgm
    };

    typedef unordered_map<string, Resource*> ResMap;

private:

    static ResMap mapRes;

    inline static void pushRes(Resource* res)
    {
        mapRes.emplace(res->resKey, res);
    }

    static ResourceType parseType(const string& path);

    static string toKey(const string& filePath);

    string resFilePath;
    string resKey;

protected:

    Resource(const string& filePath);
    virtual ~Resource();

public:

    static Resource* load(const char* filePath, ResourceType type=unknown);
    static void releaseAll();

    static Resource* get(const char* key);

    template <typename T>
    static T* get(const char* key)
    {
        return dynamic_cast<T*>(get(key));
    }

    const char* filePath();
    const char* key();

};

#endif // RESOURCE_H
