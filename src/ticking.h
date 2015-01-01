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

    void update(double deltaSec);
    double singleTick(double deltaSec);

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
