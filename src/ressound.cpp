#include "ressound.h"


ResSound::ResSound(const string& filePath, bool isStream, float volume)
    :Resource(filePath)
{
    if(isStream)
    {
        soundBuffer=new StreamBuffer();
        soundBuffer->loadWavVol(filePath.c_str(), volume);
    }
    else
    {
        soundBuffer=new SoundBuffer();
        soundBuffer->loadWavVol(filePath.c_str(), volume);
    }
}

ResSound::~ResSound()
{
    if(soundBuffer)
    {
        soundBuffer->release();
        delete soundBuffer;
        soundBuffer=0;
    }
}

