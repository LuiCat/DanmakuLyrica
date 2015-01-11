#ifndef BEATTICKING_H
#define BEATTICKING_H

#include "ticking.h"

class BeatTicking : public Ticking
{
public:

    BeatTicking(double _beatTickRate=1.0, double _secTickRate=0.0);

    void resetBeat(bool tickZero=true);

    void setBeatTickRate(double ticksPerBeat);
    void setBeatTickBeat(double beatsPerTick);

    double getTimeBeat() const;

    // update and tick depending on beats
    void updateBeat(double deltaBeat);

    // only update appropiate time and do next tick
    void singleBeatTick();
    double singleBeatTick(double deltaBeat);

    // only update appropiate time and not do next tick, normally use seekNextBeatTick() and singleBeatTick() as a pair;
    void seekNextBeatTick();
    double seekNextBeatTick(double deltaBeat);

protected:

    double timeBeat;

private:

    double beatTickRate;
    double beatTickBeat;

    double processBeat;

};

#endif // BEATTICKING_H
