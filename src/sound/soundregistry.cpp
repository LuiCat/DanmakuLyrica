#include "soundregistry.h"

void releaseFunc(SoundBuffer* p)
{
    p->release();
}

Registry<SoundBuffer> SoundRegistry::reg;

int SoundRegistry::createSound(const char *name, bool isStream, const char *filename, float volume)
{
    if(isStream)
    {
        StreamBuffer b;
        b.loadVol(filename, volume);
        return reg.registerName(name, &b);
    }
    else
    {
        SoundBuffer b;
        b.loadVol(filename, volume);
        return reg.registerName(name, &b);
    }
}

void SoundRegistry::releaseAll()
{
    reg.releaseAll(releaseFunc);
}
