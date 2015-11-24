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
    DWORD bufferSize;

    DWORD playFlag;
    DWORD loadFlag;

    bool pitchChanged;

    static bool loadWavFile(const char *filename, char **memout, DWORD *memsize, WAVEFORMATEX* format);
    static bool loadOggFile(const char *filename, char **memout, DWORD *memsize, WAVEFORMATEX* format);

    virtual bool onLoad(LPDIRECTSOUNDBUFFER primaryBuffer);

    // Prepare (copy, reset, etc.) wav data to buffer
    virtual void prepareBuffer();

public:

    explicit SoundBuffer();
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

    virtual void setPitch(double pitch);
    void resetPitch();

    void setVolume(long volume);
    void setVolume(float volume);
    void setPan(long pan);

    virtual bool load(const char *filename);

    inline bool loadVol(const char *filename, float volume)
	{
        if (!load(filename))return false;
        setVolume(volume);
        return true;
	}

};

class StreamBuffer : public SoundBuffer
{
protected:

    static DWORD streamBufSize;

    HANDLE event[MAX_NOTIFY_NUM];

    DWORD currentMemPos; // copy processing position in wav data
    DWORD lastWrittenPos; // the position in buffer to continue copying at
    //DWORD processedMemLen;

    volatile bool isPlaying;
    volatile bool paused;

    DWORD loopPosA, loopPosB;

    bool onLoad(LPDIRECTSOUNDBUFFER primaryBuffer);

    // Prepare (copy, reset, etc.) wav data to buffer
    void prepareBuffer();

    // Copy wav data to some position of buffer from currentMemPos
    void copyBuffer(void *buffer, DWORD size);

public:

    explicit StreamBuffer();
    ~StreamBuffer();

    void release();

    void setLoopPos(DWORD posA, DWORD posB);

    void play(bool restart=false);
    void stop();
    void pause();

    static DWORD WINAPI PlayThread(LPVOID lpParam);

    void setPos(DWORD npos);
    DWORD getPos() const;

};

#endif
