#ifndef BEATTIME_H
#define BEATTIME_H

struct BeatTime
{
    double sec;
    double beat;
    BeatTime(double parSec = 0.0, double parBeat = 0.0) : sec(parSec), beat(parBeat) {}

    static const BeatTime zero;

    inline BeatTime& operator*=(double k)
    {
        sec*=k;
        beat*=k;
        return *this;
    }

    inline BeatTime& operator+=(const BeatTime& t)
    {
        sec+=t.sec;
        beat+=t.beat;
        return *this;
    }

    inline BeatTime& operator-=(const BeatTime& t)
    {
        sec-=t.sec;
        beat-=t.beat;
        return *this;
    }

    inline void resizeBySec(double newSec)
    {
        beat*=(sec==0?0:newSec/sec);
        sec=newSec;
    }

    inline void resizeByBeat(double newBeat)
    {
        sec*=(beat==0?0:newBeat/beat);
        beat=newBeat;
    }

    inline void offset(double k, const BeatTime& target=BeatTime::zero)
    {
        sec+=(target.sec-sec)*k;
        beat+=(target.beat-beat)*k;
    }

    inline void offsetSec(double deltaSec, const BeatTime& target=BeatTime::zero)
    {
        beat+=(target.beat-beat)*deltaSec/(target.sec-sec);
        sec+=deltaSec;
    }

    inline void offsetBeat(double deltaBeat, const BeatTime& target=BeatTime::zero)
    {
        sec+=(target.sec-sec)*deltaBeat/(target.beat-beat);
        beat+=deltaBeat;
    }

    inline static BeatTime lerp(const BeatTime& a, const BeatTime& b, double k)
    {
        return BeatTime((1-k)*a.sec+k*b.sec, (1-k)*a.beat+k*b.beat);
    }

    inline static BeatTime lerpSec(const BeatTime& a, const BeatTime& b, double sec)
    {
        double k = (sec-a.sec)/(b.sec-a.sec);
        return BeatTime(sec, (1-k)*a.beat+k*b.beat);
    }

    inline static BeatTime lerpBeat(const BeatTime& a, const BeatTime& b, double beat)
    {
        double k = (beat-a.beat)/(b.beat-a.beat);
        return BeatTime((1-k)*a.sec+k*b.sec, beat);
    }

};

inline bool operator<(const BeatTime& a, const BeatTime& b)
{
    return a.sec<b.sec;
}

inline BeatTime operator-(const BeatTime& a, const BeatTime& b)
{
    return BeatTime(a.sec-b.sec, a.beat-b.beat);
}

typedef const BeatTime& rtime_t;

#endif // BEATTIME_H
