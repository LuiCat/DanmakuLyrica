#ifndef SOUNDS_H
#define SOUNDS_H

#include <windows.h>

#include <dsound.h>

#include <cmath>

#define MAX_NOTIFY_NUM 4

HRESULT Sound_Init(HWND hWnd);
void Sound_Cleanup();

class SoundBuffer
{
protected:

    char* m_buffer;
    DWORD m_size;

    WAVEFORMATEX waveFormat;
    LPDIRECTSOUNDBUFFER8 buffer;

    DWORD playFlag;

    static bool loadWavFile(const char *filename, char **mem, DWORD *memsize, WAVEFORMATEX* format);

    virtual void prepareBuffer();

public:

    SoundBuffer();
    virtual ~SoundBuffer();

    virtual void release();

    inline bool isAvailable() const
    {
        return !(buffer==0||m_buffer==0);
    }

    void setLoop(bool loop);

    virtual void play(bool restart=false);
    virtual void stop();
    virtual void pause();

    virtual void setPos(DWORD npos);
    virtual DWORD getPos() const;

    virtual void setTime(double timeSec);
    virtual double getTime() const;

    void setVolume(long volume);
    void setVolume(float volume);
    void setPan(long pan);

    virtual bool loadWav(const char *filename);

    inline bool loadWavVol(const char *filename, float volume)
	{
        if (!loadWav(filename))return false;
        setVolume(volume);
        return true;
	}

};

class StreamBuffer : public SoundBuffer
{
protected:

    static DWORD streamBufSize;

    HANDLE event[MAX_NOTIFY_NUM];

    DWORD currentMemPos;
    DWORD lastWrittenPos;
    DWORD processedMemLen;

    bool isPlaying;
    bool paused;

    DWORD loopPosA, loopPosB;

    void prepareBuffer();

    void copyBuffer(void *buffer, DWORD size);

public:

    StreamBuffer();
    virtual ~StreamBuffer();

    void release();

    bool loadWav(const char *filename);

    void setLoopPos(DWORD posA, DWORD posB);

    void play(bool restart=false);
    void stop();
    void pause();

    static DWORD WINAPI PlayThread(LPVOID lpParam);

    void setPos(DWORD npos);
    DWORD getPos() const;

};

#endif
