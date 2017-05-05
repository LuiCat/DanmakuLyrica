// 编码：Utf-8 without BOM
// Created by LuiCat

#ifndef _NOTEMAP_BEAT_PROVIDER_H_
#define _NOTEMAP_BEAT_PROVIDER_H_

#include "beatprovider.h"
#include "notemap.h"

class NotemapBeatProvider : public BeatProvider
{
protected:

    NoteMap* notemap;
    MapState mapState;

    bool notemapChanged;

public:

    NotemapBeatProvider(NoteMap* notemap = nullptr);

    void setNoteMap(NoteMap* notemap);

    virtual void seekTime(double timeSec) override;
    virtual double updateTime(double deltaSec) override;

    virtual double getBeat() override;
    virtual bool needSeekTime() override;

};

#endif // _NOTEMAP_BEAT_PROVIDER_H_
