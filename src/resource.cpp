#include "resource.h"

#include "resimage.h"
#include "ressound.h"

Resource::ResMap Resource::mapRes;

Resource::Resource(const string& filePath)
    :resFilePath(filePath)
{
    resKey=toKey(filePath);
}

Resource::~Resource()
{

}

Resource::ResourceType Resource::parseType(const string& path)
{
    int pos=path.find_last_of('.', -1);
    if(pos==-1)
        return unknown;
    string ext(path, pos);
    if(ext.compare(".png"))
        return image;
    if(ext.compare(".jpg"))
        return image;
    if(ext.compare(".wav"))
        return sound;
    return unknown;
}

string Resource::toKey(const string& path)
{
    string res(path, path.find_last_of('.'));
    int len=res.length();
    for(int i=0;i<len;++i)
        if(res[i]=='/'||res[i]=='\\')
            res[i]='.';
    return res;
}

Resource* Resource::load(const char* filePath, ResourceType type)
{
    string path(filePath);

    if(type==unknown)
    {
        type=parseType(path);
    }

    Resource* res=0;

    switch(type)
    {
    case image:
        res=new ResImage(path);
        break;
    case sound:
        res=new ResSound(path, false);
        break;
    case bgm:
        res=new ResSound(path, true);
        break;
    default:
        res=new Resource(path);
        break;
    }

    if(res)
        pushRes(res);

    return res;
}

void Resource::releaseAll()
{
    ResMap::iterator iter=mapRes.begin();
    for(;iter!=mapRes.end();++iter)
    {
        delete iter->second;
    }
    mapRes.clear();
}

Resource* Resource::get(const char* key)
{
    ResMap::iterator iter=mapRes.find(string(key));
    if(iter==mapRes.end())
        return 0;
    return iter->second;
}

const char* Resource::filePath()
{
    return resFilePath.c_str();
}

const char* Resource::key()
{
    return resKey.c_str();
}
