#include "soundregistry.h"

Registry<SoundBuffer> SoundRegistry::reg;

int SoundRegistry::createSound(const char *name, bool isStream, const char *filename, float volume)
{
    if(isStream)
    {
        return reg.registerName<StreamBuffer>(name, filename, volume);
    }
    else
    {
        return reg.registerName<SoundBuffer>(name, filename, volume);
    }
}

void SoundRegistry::releaseAll()
{
    reg.releaseAll([](SoundBuffer* p){p->release();});
}
