#ifndef TIMELINE_H
#define TIMELINE_H

#include "accuratetimer.h"

#include <deque>
#include <chrono>

/**
 * @brief The SmoothTimer class
 * This class receives raw delta-time and return smoothen delta-time base on real time.
 * With real-time on x-axis and parameter-time on y-axis, line of the smoothen one should
 * be smooth and close to raw one.
 */
class SmoothTimer
{
public:

    SmoothTimer();

    void setSmoothParam(double halfTime, double decaySlope);

    void reset();

    void pause();
    void resume();

    // get accurate time depend on inner timer
    double getDeltaTime();

    // get smoothen time depend on outer timers
    double getDeltaTimeFixed(double deltaSec);

protected:

    // get real time
    double timeReal();

    void resetTimeRecord();
    void insertTimeRecord(double real, double raw);

    double getTimeFixed(double real);

private:

    typedef std::chrono::steady_clock Clock;
    inline void decaySum(double& sum, double x, double dec)
    {
        sum *= dec;
        sum += x;
    }

    AccurateTimer<Clock> timer;

    // sum of unfix raw time. this will not reach MIN_TIMEFIX_STEP
    double timeUnfix;

    // progresses on real time, raw parameter time and fixed returned time
    double realStamp;
    double rawStamp;
    double fixStamp;

    // these will decay every time when insert a record
    double sumReal;
    double sumRaw;
    double sumNum;

    double slopeSum;
    double slopeNum;

    // parameters
    double ksum;
    double dslp;

    // useless now
    std::deque<std::pair<double, double>> record;

};

#endif // TIMELINE_H
