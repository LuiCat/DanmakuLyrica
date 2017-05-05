// 编码：Utf-8 without BOM
// Created by LuiCat

#ifndef _BEAT_PROVIDER_H_
#define _BEAT_PROVIDER_H_

// interface for TimeManager, provides beat according to time.
class BeatProvider
{
public:
    // time seeking function
    virtual void seekTime(double timeSec) = 0;
    // time updating function, return remaining time if multiple update is needed
    virtual double updateTime(double deltaSec) = 0;
    
    // beat provider function. this function is called after seeking/updating
    virtual double getBeat() = 0;
    // return true to trigger a time seek at next update
    virtual bool needSeekTime();
};

// example implement of BeatProvider, gives beats on linear slope
class LinearBeat : public BeatProvider
{
protected:

    double currTime;
    double currBeat;

    double beatPerSec;
    double offset;

    bool bpmChanged;

public:

    LinearBeat(double offset = 0, double bpm = 120);

    void setBpmOffset(double offset, double bpm);

    virtual void seekTime(double timeSec) override;
    virtual double updateTime(double deltaSec) override;

    virtual double getBeat() override;
    virtual bool needSeekTime() override;

};

#endif // _BEAT_PROVIDER_H_
