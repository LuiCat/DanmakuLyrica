#include "sounds.h"

#include <mmsystem.h>
#include <cmath>

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "debug.h"

//=============Globals============================

LPDIRECTSOUND8       lpDirectSound = NULL;

HRESULT Sound_Init(HWND hWnd)
{
    HRESULT hr;
    DirectSoundCreate8(NULL, &lpDirectSound, NULL);
    hr = lpDirectSound->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE /*DSSCL_PRIORITY*/);
    if(FAILED(hr))
        return hr;
    return S_OK;
}

void Sound_Cleanup()
{
    if(lpDirectSound)
        lpDirectSound->Release();
}

//=============Sound Members============================

bool Sound::loadWavFile(const char *filename, char **memout, DWORD *memsize, WAVEFORMATEX* format)
{
    if(!memout)
        return false;

    HMMIO           hwav;    // handle to wave file
    MMCKINFO        parent,  // parent chunk
                    child;   // child chunk

    WAVEFORMATEX    wfmtx;

    parent.ckid         = (FOURCC)0;
    parent.cksize       = 0;
    parent.fccType      = (FOURCC)0;
    parent.dwDataOffset = 0;
    parent.dwFlags      = 0;
    child = parent;

    // open the WAV file
    if ((hwav = mmioOpen(const_cast<char*>(filename), NULL, MMIO_READ | MMIO_ALLOCBUF))==NULL)
        return false;

    // descend into the RIFF
    parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF)){
        mmioClose(hwav, 0);
        return false;
    }

    // descend to the WAVEfmt
    child.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hwav, &child, &parent, 0)){
        mmioClose(hwav, 0);
        return false;
    }

    // now read the wave format information from file
    if (mmioRead(hwav, (char *)&wfmtx, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX)){
        mmioClose(hwav, 0);
        return false;
    }

    // make sure that the data format is PCM
    if (wfmtx.wFormatTag != WAVE_FORMAT_PCM){
        mmioClose(hwav, 0);
        return false;
    }

    // now ascend up one level, so we can access data chunk
    if (mmioAscend(hwav, &child, 0)){
        mmioClose(hwav, 0);
        return false;
    }

    // descend to the data chunk
    child.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
    {
        mmioClose(hwav, 0);
        return false;
    }

    // finally!!!! now all we have to do is read the data in and
    // set up the directsound buffer

    if(*memout)
        delete[] *memout;

    *memsize=child.cksize-4;
    *memout=new char[child.cksize-4];
    *format=wfmtx;

    mmioSeek(hwav, 4, SEEK_CUR);
    mmioRead(hwav, (char*)*memout, child.cksize-4);
    mmioClose(hwav, 0);

    return true;
}

bool Sound::loadOggFile(const char* filename, char** memout, DWORD* memsize, WAVEFORMATEX* format)
{
    if(!memout)
        return false;

    FILE* f=fopen(filename, "rb");
    if(!f)return false;

    OggVorbis_File vorbisFile;
    memset(&vorbisFile, 0, sizeof(vorbisFile));
    ov_clear(&vorbisFile);

    if(ov_open_callbacks(f, &vorbisFile, 0, 0, OV_CALLBACKS_DEFAULT))
    {
        ov_clear(&vorbisFile);
        return false;
    }

    vorbis_info *vi=ov_info(&vorbisFile, -1);

    WAVEFORMATEX wfmtx;
    memset(&wfmtx, 0, sizeof(wfmtx));

    wfmtx.wFormatTag=WAVE_FORMAT_PCM;
    wfmtx.nChannels=vi->channels;
    wfmtx.nSamplesPerSec=vi->rate;
    wfmtx.nAvgBytesPerSec=vi->channels*vi->rate*2;
    wfmtx.wBitsPerSample=16;
    wfmtx.nBlockAlign=vi->channels*2;
    wfmtx.cbSize=0;

    int current_section;
    int remainSize=2*vi->channels*(int)ov_pcm_total(&vorbisFile, -1);
    int readSize;
    char* p=new char[remainSize];

    if(*memout)
        delete[] *memout;
    *memout=p;
    *memsize=remainSize;
    *format=wfmtx;

    while(true)
    {
        readSize=ov_read(&vorbisFile, p, remainSize, 0, 2, 1, &current_section);
        if(readSize<=0)
            break;
        p+=readSize;
        remainSize-=readSize;
    }

    ov_clear(&vorbisFile);

    return true;
}

bool Sound::load(const char *filename)
{
    if(isAvailable())
        release();

    int dpos=strlen(filename)-1;
    while(dpos>=0&&filename[dpos]!='.')
        --dpos;
    if(dpos>0&&stricmp(filename+dpos, ".wav")==0)
    {
        if(!loadWavFile(filename, &m_buffer, &m_size, &waveFormat))
            return false;
    }
    else if(dpos>0&&stricmp(filename+dpos, ".ogg")==0)
    {
        if(!loadOggFile(filename, &m_buffer, &m_size, &waveFormat))
            return false;
    }
    else
    {
        return false;
    }

    DSBUFFERDESC dsbdesc;
    memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = loadFlag;
    dsbdesc.dwBufferBytes = (bufferSize==0?m_size:bufferSize);
    dsbdesc.lpwfxFormat = &waveFormat;

    LPDIRECTSOUNDBUFFER primaryBuffer;
    if(FAILED(lpDirectSound->CreateSoundBuffer(&dsbdesc, &primaryBuffer, NULL)))
        return false;

    if(FAILED(primaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffer)))
        return false;

    if(!onLoad(primaryBuffer))
        return false;

    primaryBuffer->Release();

    return true;
}

bool Sound::onLoad(LPDIRECTSOUNDBUFFER)
{
    prepareBuffer();
    return true;
}

void Sound::prepareBuffer()
{
    void* buffer1;
    DWORD bufferSize1;

    if (buffer->Lock(0, 0, &buffer1, &bufferSize1, 0, 0, DSBLOCK_ENTIREBUFFER)==S_OK)
    {
        memcpy(buffer1, m_buffer, bufferSize1);
        buffer->Unlock(buffer1, bufferSize1, NULL, 0);
    }
}

Sound::Sound()
    :m_buffer(0)
    ,m_size(0)
    ,buffer(0)
    ,bufferSize(0)
    ,playFlag(0)
    ,loadFlag(DSBCAPS_GLOBALFOCUS|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY)//DSBCAPS_CTRLFREQUENCY
    ,pitchChanged(false)
{
    memset(&waveFormat, 0, sizeof(waveFormat));
}

Sound::~Sound()
{

}

Sound::Sound(const char* filename, float volume)
    : Sound()
{
    loadVol(filename, volume);
}

void Sound::release()
{
    if(buffer)
    {
        buffer->Release();
        buffer=0;
    }
    if(m_buffer)
    {
        delete[] m_buffer;
        m_buffer=0;
    }
}

void Sound::setLoop(bool loop)
{
    if(loop)
        playFlag|=DSBPLAY_LOOPING;
    else
        playFlag&=~DSBPLAY_LOOPING;
}

void Sound::play(bool restart)
{
    if(!isAvailable())return;
    if(restart)
        buffer->SetCurrentPosition(0);
    buffer->Play(0, 0, playFlag);
}

void Sound::stop()
{
    if(!isAvailable())return;
    buffer->Stop();
    buffer->SetCurrentPosition(0);
}

void Sound::pause()
{
    if(!isAvailable())return;
    buffer->Stop();
}

bool Sound::isPlaying()
{
    return getPos() == 0;
}

void Sound::setPos(long npos)
{
    if(!isAvailable())return;
    if(npos<0)npos=0;
    npos=npos/waveFormat.wBitsPerSample*waveFormat.wBitsPerSample;
    buffer->SetCurrentPosition(npos);
}

long Sound::getPos() const
{
    if (!isAvailable())return 0;
    DWORD playPos;
    buffer->GetCurrentPosition(&playPos, 0);
    return playPos;
}

void Sound::setTime(double timeSec)
{
    setPos(static_cast<DWORD>(timeSec * waveFormat.nAvgBytesPerSec));
}

double Sound::getTime() const
{
    return (double)getPos() / waveFormat.nAvgBytesPerSec;
}

void Sound::setPitch(double pitch)
{
    if(!isAvailable())return;
    buffer->SetFrequency(static_cast<DWORD>(waveFormat.nSamplesPerSec * pitch));
    pitchChanged = true;
}

void Sound::resetPitch()
{
    if(pitchChanged)
    {
        setPitch(1.0);
        pitchChanged = false;
    }
}

void Sound::setVolume(long volume)
{
    if(!isAvailable())return;
    buffer->SetVolume(volume);
}

void Sound::setVolume(float volume)
{
    if(!isAvailable())return;
    buffer->SetVolume(static_cast<DWORD>(2000.0 * log10(volume)));
}

void Sound::setPan(long pan)
{
    if(!isAvailable())return;
    buffer->SetPan(pan);
}

//=====================StreamSound Members=============================================

DWORD StreamSound::streamBufSize=88200;

StreamSound::StreamSound()
    :Sound()
    ,currentMemPos(0)
    ,lastWrittenPos(0)
    ,playing(false)
    ,paused(false)
    ,ended(false)
    ,loopPosA(0)
    ,loopPosB(0)
{
    setLoop(true);
    memset(event, 0, sizeof(event));
    bufferSize=streamBufSize;
    loadFlag|=DSBCAPS_CTRLPOSITIONNOTIFY;
}

StreamSound::~StreamSound()
{

}

StreamSound::StreamSound(const char* filename, float volume)
{
    loadVol(filename, volume);
}

void StreamSound::release()
{
    if(playing)
        stop();
    for(int i=0;i<MAX_NOTIFY_NUM;++i)
    {
        if(event[i]==0)
            continue;
        CloseHandle(event[i]);
        event[i]=0;
    }
    Sound::release();
}

void StreamSound::setLoopPos(long posA, long posB)
{
    loopPosA=posA;
    loopPosB=posB;
}

DWORD WINAPI StreamSound::PlayThread(LPVOID lpParam)
{
    StreamSound* instance=(StreamSound*)lpParam;

    instance->buffer->Play(0, 0, DSBPLAY_LOOPING);

    WaitForSingleObject(instance->event[0], INFINITE);
    while(instance->playing)
    {
        WaitForMultipleObjects(MAX_NOTIFY_NUM, instance->event, false, INFINITE);
        instance->prepareBuffer();
    }

    return 0;
}

bool StreamSound::onLoad(LPDIRECTSOUNDBUFFER primaryBuffer)
{
    DSBPOSITIONNOTIFY     posNotify[MAX_NOTIFY_NUM];
    LPDIRECTSOUNDNOTIFY   notify = 0;

    for(int i=0; i<MAX_NOTIFY_NUM; ++i)
    {
        posNotify[i].dwOffset=i*streamBufSize/MAX_NOTIFY_NUM;
        posNotify[i].hEventNotify=event[i]=CreateEvent(0, false, false, 0);
    }

    HRESULT hr;

    if(FAILED(hr=primaryBuffer->QueryInterface(IID_IDirectSoundNotify8, (LPVOID*)&notify)))
        return false;

    hr=notify->SetNotificationPositions(MAX_NOTIFY_NUM, posNotify);
    notify->Release();

    return true;
}

void StreamSound::prepareBuffer()
{
    // This has moved into StreamSound::copyBuffer
    /*
    if(currentMemPos>=m_size)
    {
        stop();
        return;
    }
    */

    HRESULT hr;

    void* buffer1=NULL;
    void* buffer2=NULL;
    DWORD bufferSize1=0;
    DWORD bufferSize2=0;

    DWORD currentPos;

    buffer->GetCurrentPosition(&currentPos, 0);

    DWORD pendingIndex=lastWrittenPos;
    DWORD pendingSize=(playing?(currentPos+streamBufSize-lastWrittenPos)%streamBufSize:streamBufSize);

    if(pendingSize==0)
        return;

    hr=buffer->Lock(pendingIndex, pendingSize, &buffer1, &bufferSize1, &buffer2, &bufferSize2, 0);
    if(hr==DSERR_BUFFERLOST)
    {
        buffer->Restore();
        hr=buffer->Lock(pendingIndex, pendingSize, &buffer1, &bufferSize1, &buffer2, &bufferSize2, 0);
    }

    if(hr!=S_OK)
    {
        stop();
        cout<<"Failed locking sound buffer"<<endl;
        return;
    }

    copyBuffer(buffer1, bufferSize1);

    if (buffer2)
        copyBuffer(buffer2, bufferSize2);

    lastWrittenPos%=streamBufSize;

    hr=buffer->Unlock(buffer1, bufferSize1, buffer2, bufferSize2);

}

void StreamSound::copyBuffer(void* buffer, DWORD size)
{
    ZeroMemory(buffer, size);

    if (currentMemPos >= (long)m_size)
    {
        ended = true;
        return;
    }

    DWORD tempBufferSize;
    char* cBuffer=(char*)buffer;

    while(size>0)
    {
        tempBufferSize=(loopPosB>0?loopPosB:m_size)-currentMemPos;
        if(tempBufferSize>size)
            tempBufferSize=size;

        if(currentMemPos<0)
        {
            if((long)tempBufferSize+currentMemPos>0)
                memcpy(cBuffer-currentMemPos, m_buffer, tempBufferSize+currentMemPos);
        }
        else
        {
            memcpy(cBuffer, m_buffer+currentMemPos, tempBufferSize);
        }

        size-=tempBufferSize;
        cBuffer+=tempBufferSize;

        currentMemPos+=tempBufferSize;
        lastWrittenPos+=tempBufferSize;
        //processedMemLen+=tempBufferSize;

        if(loopPosB==0 || loopPosB>m_size || loopPosA>=loopPosB)
            break;

        if(currentMemPos==loopPosB)
        {
            currentMemPos=loopPosA;
        }
    }

}

void StreamSound::play(bool restart)
{
    if(!isAvailable())return;
    if(restart)
    {
        stop();
    }

    ended = false;

    prepareBuffer();

    if(!playing)
    {
        CloseHandle(CreateThread(0, 0, StreamSound::PlayThread, this, 0, 0));
        playing=true;
        paused=false;
    }
    else
    {
        buffer->Play(0, 0, DSBPLAY_LOOPING);
        paused=false;
    }
}

void StreamSound::stop()
{
    if(!isAvailable())return;
    buffer->SetCurrentPosition(0);
    playing=false;
    paused=false;
    currentMemPos=0;
    lastWrittenPos=0;
    //processedMemLen=0;
}

void StreamSound::pause()
{
    if(!isAvailable())return;
    buffer->Stop();
    paused=true;
}

bool StreamSound::isPlaying()
{
    return playing && !ended;
}

void StreamSound::setPos(long npos)
{
    if(!isAvailable())return;
    npos=npos/waveFormat.wBitsPerSample*waveFormat.wBitsPerSample;
    bool flag=(playing&&!paused);
    if(flag)
        pause();
    currentMemPos=npos;
    lastWrittenPos=0;
    if(flag)
    {
        playing=false;
        prepareBuffer();
        playing=true;
        play();
    }
}

long StreamSound::getPos() const
{
    if(!playing)
        return currentMemPos;
    DWORD pos;
    buffer->GetCurrentPosition(&pos, 0);
    return currentMemPos-(lastWrittenPos>pos?lastWrittenPos-pos:lastWrittenPos+streamBufSize-pos);
}
