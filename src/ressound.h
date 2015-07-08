#ifndef RESSOUND_H
#define RESSOUND_H

#include "resource.h"
#include "sounds.h"

#define SOUND(s) ((SoundBuffer*)Resource::get<ResSound>(s))

class ResSound : public Resource
{
private:

    SoundBuffer* soundBuffer;

protected:

    explicit ResSound(const string& filePath, bool isStream=false, float volume=1.0);
    ~ResSound();

    friend class Resource;

public:

    inline SoundBuffer* operator->() const
    {
        return soundBuffer;
    }

    inline operator SoundBuffer*() const
    {
        return soundBuffer;
    }

};

#endif // RESSOUND_H
