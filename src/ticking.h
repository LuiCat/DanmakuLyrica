#ifndef TICKING_H
#define TICKING_H

class Ticking
{
public:

    Ticking();

    void setDead();

    bool dead() const;

    int getTick() const;
    double getSec() const;

    virtual void update(double deltaSec);

protected:

    bool isDead;

    int tick;
    double timeSec;
    double tickRate;
    double nextTickSec;

    bool useDefaultTicking;

    virtual void onUpdateMotion(double deltaSec, double deltaTick);
    virtual void onTick();

};

#endif // TICKING_H
