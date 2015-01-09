#ifndef TICKING_H
#define TICKING_H

class Ticking
{
public:

    Ticking(double _tickRate=0.0);

    void setDead();
    bool dead() const;
    void reset(bool tickZero=true);

    void setTickRate(double ticksPerSec);
    void setTickSec(double secsPerTick);

    int getTick() const;
    double getTimeSec() const;

    // update and do ticks
    void update(double deltaSec);

    // only update appropiate time and do next tick
    void singleTick();
    double singleTick(double deltaSec);

    // only update appropiate time and not do next tick, normally use seekNextTick() and singleTick() as a pair;
    void seekNextTick();
    double seekNextTick(double deltaSec);

protected:

    bool isDead;

    int tick;
    double timeSec;

    bool useDefaultTicking;

    virtual void onUpdateMotion(double deltaSec, double deltaTick);
    virtual void onTick();

private:

    double tickRate;
    double tickSec;

    double processSec;

};

#endif // TICKING_H
