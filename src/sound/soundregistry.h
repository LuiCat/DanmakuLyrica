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

    static int createSound(const char* name, bool isStream, const char* filename, float volume);

    template <typename SoundType>
    static int createSound(const char* name, const char* filename, float volume)
    {
        static_assert(is_base_of<SoundBuffer, SoundType>::value,
                      "SoundType does not implement SoundBuffer in \
                       SoundRegistry::createSound<typename SoundType>");
        return reg.registerName<SoundType>(name, filename, volume);
    }

    static void releaseAll();

    inline static SoundBuffer* get(int id)
    {
        return SoundRegistry::reg.getInfo(id);
    }

    inline static SoundBuffer* get(const char* name)
    {
        return SoundRegistry::reg.getInfo(SoundRegistry::reg.getId(name));
    }

    inline static int getID(const char* name)
    {
        return SoundRegistry::reg.getId(name);
    }

};

#endif // SOUNDREGITRY_H
