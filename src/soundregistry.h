#ifndef SOUNDREGITRY_H
#define SOUNDREGITRY_H

#include "registry.h"
#include "sounds.h"

#define SOUND(x) (SoundRegistry::get(x))

class SoundRegistry
{
public:
    static Registry<SoundBuffer> reg;
    static void registerSoundFile(const char* name, bool isStream, const char* filename, float volume);
    static void releaseAllSounds();
    static SoundBuffer* get(int id);
    static SoundBuffer* get(const char* name);
};

#endif // SOUNDREGITRY_H
