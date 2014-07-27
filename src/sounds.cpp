#include "sounds.h"

#include <mmsystem.h>
#include <cmath>

//=============Globals============================

LPDIRECTSOUND8       lpDirectSound = NULL;

HRESULT Sound_Init(HWND hWnd)
{
    HRESULT hr;
    DirectSoundCreate8(NULL, &lpDirectSound, NULL);
    hr = lpDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
    if(FAILED(hr))
        return hr;
    return S_OK;
}

void Sound_Cleanup()
{
    if(lpDirectSound)
        lpDirectSound->Release();
}

//=============SoundBuffer Members============================

bool SoundBuffer::loadWavFile(const char *filename, char **mem, DWORD *memsize, WAVEFORMATEX* format)
{
    if(!mem)
        return false;

    HMMIO           hwav;    // handle to wave file
    MMCKINFO        parent,  // parent chunk
                    child;   // child chunk

    WAVEFORMATEX    wfmtx;

    CHAR            namestr[100];

    parent.ckid         = (FOURCC)0;
    parent.cksize       = 0;
    parent.fccType      = (FOURCC)0;
    parent.dwDataOffset = 0;
    parent.dwFlags      = 0;
    child = parent;

    strcpy(namestr, filename);

    // open the WAV file
    if ((hwav = mmioOpen(namestr, NULL, MMIO_READ | MMIO_ALLOCBUF))==NULL)
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

    *memsize=child.cksize-4;
    *mem=new char[child.cksize-4];
    *format=wfmtx;

    mmioSeek(hwav, 4, SEEK_CUR);
    mmioRead(hwav, (char*)*mem, child.cksize-4);
    mmioClose(hwav, 0);

    return true;
}

bool SoundBuffer::loadWav(const char *filename)
{
    if(m_buffer || buffer)
        release();

    if(!loadWavFile(filename, &m_buffer, &m_size, &waveFormat))
        return false;

    DSBUFFERDESC dsbdesc;
    memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;//DSBCAPS_CTRLFREQUENCY;
    dsbdesc.dwBufferBytes = m_size;
    dsbdesc.lpwfxFormat = &waveFormat;

    LPDIRECTSOUNDBUFFER primaryBuffer;
    if(FAILED(lpDirectSound->CreateSoundBuffer(&dsbdesc, &primaryBuffer, NULL)))
        return false;

    if(FAILED(primaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffer)))
        return false;

    primaryBuffer->Release();

    void* buffer1;
    DWORD bufferSize1;

    if (buffer->Lock(0, 0, &buffer1, &bufferSize1, 0, 0, DSBLOCK_ENTIREBUFFER)==S_OK)
    {
        memcpy(buffer1, m_buffer, bufferSize1);
        buffer->Unlock(buffer1, bufferSize1, NULL, 0);
    }

    return true;
}

void SoundBuffer::prepareBuffer()
{
    void* buffer1;
    DWORD bufferSize1;

    if (buffer->Lock(0, 0, &buffer1, &bufferSize1, 0, 0, DSBLOCK_ENTIREBUFFER)==S_OK)
    {
        memcpy(buffer1, m_buffer, bufferSize1);
        buffer->Unlock(buffer1, bufferSize1, NULL, 0);
    }
}

SoundBuffer::SoundBuffer()
{
    playFlag=0;
    m_size=0;
    m_buffer=0;
    buffer=0;
}

SoundBuffer::~SoundBuffer()
{
}

void SoundBuffer::release()
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

void SoundBuffer::setLoop(bool loop)
{
    if(loop)
        playFlag|=DSBPLAY_LOOPING;
    else
        playFlag&=~DSBPLAY_LOOPING;
}

void SoundBuffer::play(bool restart)
{
    if(!isAvailable())return;
    if(restart)
        buffer->SetCurrentPosition(0);
    buffer->Play(0, 0, playFlag);
}

void SoundBuffer::stop()
{
    if(!isAvailable())return;
    buffer->Stop();
    buffer->SetCurrentPosition(0);
}

void SoundBuffer::pause()
{
    if(!isAvailable())return;
    buffer->Stop();
}

void SoundBuffer::setPos(DWORD npos)
{
    if(!isAvailable())return;
    buffer->SetCurrentPosition(npos);
}

DWORD SoundBuffer::getPos() const
{
    if (!isAvailable())return 0;
    DWORD playPos;
    buffer->GetCurrentPosition(&playPos, 0);
    return playPos;
}

void SoundBuffer::setTime(double time)
{
    setPos(time*44.1*4);
}

double SoundBuffer::getTime() const
{
    return getPos() / 44.1 / 4;
}

void SoundBuffer::setVolume(long volume)
{
    if(!isAvailable())return;
    buffer->SetVolume(volume);
}

void SoundBuffer::setVolume(float volume)
{
    if(!isAvailable())return;
    buffer->SetVolume(2000.0 * log10(volume));
}

void SoundBuffer::setPan(long pan)
{
    if(!isAvailable())return;
    buffer->SetPan(pan);
}

//=====================StreamBuffer Members=============================================

DWORD StreamBuffer::streamBufSize=88200;

StreamBuffer::StreamBuffer()
    :SoundBuffer()
{
    setLoop(true);
    loopPosA=loopPosB=0;
}

StreamBuffer::~StreamBuffer()
{
}

void StreamBuffer::release()
{
    if(isPlaying)
        stop();
    for(int i=0;i<MAX_STREAM_BUF;++i)
        CloseHandle(event[i]);
    SoundBuffer::release();
}

bool StreamBuffer::loadWav(const char *filename)
{
    if(isAvailable())
        release();

    if(!loadWavFile(filename, &m_buffer, &m_size, &waveFormat))
        return false;

    DSBUFFERDESC dsbdesc;
    memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY;//DSBCAPS_CTRLFREQUENCY;
    dsbdesc.dwBufferBytes = MAX_STREAM_BUF*streamBufSize;
    dsbdesc.lpwfxFormat = &waveFormat;

    LPDIRECTSOUNDBUFFER primaryBuffer;

    if(FAILED(lpDirectSound->CreateSoundBuffer(&dsbdesc, &primaryBuffer, NULL)))
        return false;

    if(FAILED(primaryBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&buffer)))
        return false;

    /*
    void* lpvWrite;
    DWORD  dwLength;

    if (buffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, 0, DSBLOCK_ENTIREBUFFER)==S_OK)
    {
      memcpy(lpvWrite, m_buffer, dwLength);
      buffer->Unlock(lpvWrite, dwLength, NULL, 0);
    }
    */

    DSBPOSITIONNOTIFY     posNotify[MAX_STREAM_BUF];
    LPDIRECTSOUNDNOTIFY   notify = 0;

    for(int i=0; i<MAX_STREAM_BUF; ++i)
    {
        posNotify[i].dwOffset=i*streamBufSize;
        posNotify[i].hEventNotify=event[i]=CreateEvent(0, false, false, 0);
    }

    HRESULT hr;

    if(FAILED(hr=primaryBuffer->QueryInterface(IID_IDirectSoundNotify8, (LPVOID*)&notify)))
        return false;

    hr=notify->SetNotificationPositions(MAX_STREAM_BUF, posNotify);
    notify->Release();

    primaryBuffer->Release();

    return true;
}

void StreamBuffer::setLoopPos(DWORD posA, DWORD posB)
{
    loopPosA=posA;
    loopPosB=posB;
}

DWORD WINAPI StreamBuffer::PlayThread(LPVOID lpParam)
{
    DWORD res;
    StreamBuffer* instance=(StreamBuffer*)lpParam;

    instance->prepareBuffer();
    instance->buffer->Play(0, 0, DSBPLAY_LOOPING);
    res=WaitForSingleObject(instance->event[0], INFINITE);
    while(instance->isPlaying)
    {
        res=WaitForMultipleObjects(MAX_STREAM_BUF, instance->event, false, INFINITE);
        instance->processBuffer(res-WAIT_OBJECT_0);
    }

    return 0;
}

void StreamBuffer::prepareBuffer()
{
    void* buffer1;
    DWORD bufferSize1;

    currentMemPos=m_buffer;
    remainMemSize=(loopPosB>0&&loopPosB<m_size?loopPosB:m_size);
    lastWrittenMemIdx=0;

    if (buffer->Lock(0, 0, &buffer1, &bufferSize1, 0, 0, DSBLOCK_ENTIREBUFFER)==S_OK)
    {
        copyBuffer(buffer1, bufferSize1);
        buffer->Unlock(buffer1, bufferSize1, NULL, 0);
    }

}

void StreamBuffer::processBuffer(int i)
{
    if(remainMemSize<=0)
    {
        stop();
        return;
    }

    void* buffer1 = NULL;
    void* buffer2 = NULL;
    DWORD bufferSize1;
    DWORD bufferSize2;

    DWORD pendingIndex=(lastWrittenMemIdx%MAX_STREAM_BUF)*streamBufSize;
    DWORD pendingSize=(i-lastWrittenMemIdx+MAX_STREAM_BUF)%MAX_STREAM_BUF*streamBufSize;

    HRESULT hr;

    hr=buffer->Lock(pendingIndex, pendingSize, &buffer1, &bufferSize1, &buffer2, &bufferSize2, 0);
    if(hr==DSERR_BUFFERLOST)
    {
        buffer->Restore();
        hr=buffer->Lock(pendingIndex, pendingSize, &buffer1, &bufferSize1, &buffer2, &bufferSize2, 0);
    }

    //cout<<i<<' '<<lastWrittenMemIdx<<' '<<buffer1<<' '<<bufferSize1<<' '<<buffer2<<' '<<bufferSize2<<endl;
    lastWrittenMemIdx=i;

    if(SUCCEEDED(hr))
    {
        copyBuffer(buffer1, bufferSize1);

        if (buffer2)
            copyBuffer(buffer2, bufferSize2);

        hr=buffer->Unlock(buffer1, bufferSize1, buffer2, bufferSize2);
    }
}

void StreamBuffer::copyBuffer(void* buffer, DWORD size)
{
    ZeroMemory(buffer, size);

    DWORD tempBufferSize;
    char* cBuffer=(char*)buffer;

    while(size>0)
    {
        tempBufferSize=(remainMemSize<size?remainMemSize:size);
        memcpy(cBuffer, currentMemPos, tempBufferSize);
        size-=tempBufferSize;
        cBuffer+=tempBufferSize;

        remainMemSize-=tempBufferSize;
        currentMemPos+=tempBufferSize;

        if(loopPosB==0 || loopPosB>m_size || loopPosA>=loopPosB)
            break;

        if(remainMemSize==0)
        {
            remainMemSize=loopPosB-loopPosA;
            currentMemPos=m_buffer+loopPosA;
        }
    }

}

void StreamBuffer::play(bool restart)
{
    if(!isAvailable())return;
    if(restart)
    {
        stop();
    }
    if(!isPlaying)
    {
        CloseHandle(CreateThread(0, 0, StreamBuffer::PlayThread, this, 0, 0));
        isPlaying=true;
        paused=false;
    }
    else
    {
        buffer->Play(0, 0, DSBPLAY_LOOPING);
        paused=false;
    }
}

void StreamBuffer::stop()
{
    if(!isAvailable())return;
    buffer->Stop();
    buffer->SetCurrentPosition(0);
    isPlaying=false;
    paused=false;
}

void StreamBuffer::pause()
{
    if(!isAvailable())return;
    buffer->Stop();
    paused=true;
}
