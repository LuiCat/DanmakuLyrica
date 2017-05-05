#ifndef SOUNDREGITRY_H
#define SOUNDREGITRY_H

#include "registry.h"
#include "sounds.h"

#define SOUND(x) (SoundRegistry::get(x))

class SoundRegistry
{
private:

    static Registry<Sound> reg;

public:

    static int createSound(const char* name, bool isStream, const char* filename, float volume);

    template <typename SoundType>
    static int createSound(const char* name, const char* filename, float volume)
    {
        static_assert(is_base_of<Sound, SoundType>::value,
                      "SoundType does not implement Sound in \
                       SoundRegistry::createSound<typename SoundType>");
        return reg.registerName<SoundType>(name, filename, volume);
    }

    static void releaseAll();

    inline static Sound* get(int id)
    {
        return SoundRegistry::reg.getInfo(id);
    }

    inline static Sound* get(const char* name)
    {
        return SoundRegistry::reg.getInfo(SoundRegistry::reg.getId(name));
    }

    inline static int getID(const char* name)
    {
        return SoundRegistry::reg.getId(name);
    }

};

#endif // SOUNDREGITRY_H
