#include "soundregistry.h"

void releaseFunc(SoundBuffer* p)
{
    p->release();
}

Registry<SoundBuffer> SoundRegistry::reg;

void SoundRegistry::registerSoundFile(const char *name, bool isStream, const char *filename, float volume)
{
    if(isStream)
    {
        StreamBuffer b;
        b.loadWavVol(filename, volume);
        reg(name, &b);
    }
    else
    {
        SoundBuffer b;
        b.loadWavVol(filename, volume);
        reg(name, &b);
    }
}

void SoundRegistry::releaseAllSounds()
{
    reg.releaseAll(releaseFunc);
}

SoundBuffer *SoundRegistry::get(int id)
{
    return SoundRegistry::reg.getInfo(id);
}

SoundBuffer *SoundRegistry::get(const char *name)
{
    return get(SoundRegistry::reg.getId(name));
}
