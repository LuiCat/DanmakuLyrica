#ifndef SOUNDREGITRY_H
#define SOUNDREGITRY_H

#include "registry.h"
#include "sounds.h"

#define SOUND(x) (SoundRegistry::get(x))

class SoundRegistry
{
private:

    static Registry<SoundBuffer> reg;

public:

    static void newSound(const char* name, bool isStream, const char* filename, float volume);

    template <typename SoundType>
    static void newSound(const char* name, const char* filename, float volume)
    {
        static_assert(is_base_of<SoundType, SoundBuffer>::value,
                      "SoundType does not implement SoundBuffer in \
                       SoundRegistry::newSound<typename SoundType>");
        SoundType b;
        b.loadVol(filename, volume);
        reg(name, &b);
    }

    static void releaseAll();

    static SoundBuffer* get(int id);
    static SoundBuffer* get(const char* name);

};

#endif // SOUNDREGITRY_H
