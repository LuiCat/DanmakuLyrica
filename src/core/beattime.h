#ifndef BEATTIME_H
#define BEATTIME_H

struct BeatTimeVec
{
    double time;
    double beat;
    BeatTimeVec(double parSec = 0.0, double parBeat = 0.0) : time(parSec), beat(parBeat) {}

    static const BeatTimeVec zero;

    inline BeatTimeVec& operator*=(double k)
    {
        time*=k;
        beat*=k;
        return *this;
    }

    inline BeatTimeVec& operator+=(const BeatTimeVec& t)
    {
        time+=t.time;
        beat+=t.beat;
        return *this;
    }

    inline BeatTimeVec& operator-=(const BeatTimeVec& t)
    {
        time-=t.time;
        beat-=t.beat;
        return *this;
    }

    inline void resizeBySec(double newSec)
    {
        beat*=(time==0?0:newSec/time);
        time=newSec;
    }

    inline void resizeByBeat(double newBeat)
    {
        time*=(beat==0?0:newBeat/beat);
        beat=newBeat;
    }

    inline void offset(double k, const BeatTimeVec& target=BeatTimeVec::zero)
    {
        time+=(target.time-time)*k;
        beat+=(target.beat-beat)*k;
    }

    inline void offsetSec(double deltaSec, const BeatTimeVec& target=BeatTimeVec::zero)
    {
        beat+=(target.beat-beat)*deltaSec/(target.time-time);
        time+=deltaSec;
    }

    inline void offsetBeat(double deltaBeat, const BeatTimeVec& target=BeatTimeVec::zero)
    {
        time+=(target.time-time)*deltaBeat/(target.beat-beat);
        beat+=deltaBeat;
    }

    inline static BeatTimeVec lerp(const BeatTimeVec& a, const BeatTimeVec& b, double k)
    {
        return BeatTimeVec((1-k)*a.time+k*b.time, (1-k)*a.beat+k*b.beat);
    }

    inline static BeatTimeVec lerpSec(const BeatTimeVec& a, const BeatTimeVec& b, double time)
    {
        double k = (time-a.time)/(b.time-a.time);
        return BeatTimeVec(time, (1-k)*a.beat+k*b.beat);
    }

    inline static BeatTimeVec lerpBeat(const BeatTimeVec& a, const BeatTimeVec& b, double beat)
    {
        double k = (beat-a.beat)/(b.beat-a.beat);
        return BeatTimeVec((1-k)*a.time+k*b.time, beat);
    }

};

inline bool operator<(const BeatTimeVec& a, const BeatTimeVec& b)
{
    return a.time<b.time;
}

inline BeatTimeVec operator-(const BeatTimeVec& a, const BeatTimeVec& b)
{
    return BeatTimeVec(a.time-b.time, a.beat-b.beat);
}

typedef const BeatTimeVec& rtime_t;

class BeatTime
{
protected:

    double currTime;
    double currBeat;

    double deltaTime;
    double deltaBeat;

    BeatTime(double time, double beat, double dTime, double dBeat);

public:

    BeatTime(double timeSec = 0.0, double beat = 0.0);
    BeatTime(const BeatTime& other);

    // Getter functions

    inline double getTime() const { return currTime; }
    inline double getBeat() const { return currBeat; }
    inline double getDeltaTime() const { return deltaTime; }
    inline double getDeltaBeat() const { return deltaBeat; }

    // Functions that increases delta values.
    // Use this only if you know what you are doing !

    void addOffset(double offsetTime, double offsetBeat);
    void setOffset(double offsetTime, double offsetBeat);
    void setTarget(double targetTime, double targetBeat);

    // Functions that proceed delta values to time values.
    // A parameter larger than current delta value will be truncated.
    // Beat value is calculated in proportion giving time parameter, vice versa.

    void proceedTime(double time);
    void proceedBeat(double beat);
    void proceed();

    // Functions that split current time record to a new object.
    // Delta values can be shrinked in need.

    BeatTime splitTime(double offsetTime);
    BeatTime splitBeat(double offsetBeat);
    BeatTime split();

    friend inline bool operator<(const BeatTime& a, const BeatTime& b)
    {
        return a.currTime < b.currTime;
    }

    friend class TimeManager;

};


#endif // BEATTIME_H
