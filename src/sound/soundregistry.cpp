#include "soundregistry.h"

Registry<Sound> SoundRegistry::reg;

int SoundRegistry::createSound(const char *name, bool isStream, const char *filename, float volume)
{
    if(isStream)
    {
        return reg.registerName<StreamSound>(name, filename, volume);
    }
    else
    {
        return reg.registerName<Sound>(name, filename, volume);
    }
}

void SoundRegistry::releaseAll()
{
    reg.releaseAll([](Sound* p){p->release();});
}
