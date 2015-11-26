#ifndef ACCURATETIMER_H
#define ACCURATETIMER_H

#include <chrono>

class AccurateTimer
{
public:

    AccurateTimer();

    void restart();
    void pause();
    void resume();

    double elapsed();

    inline double operator()()
    {
        return elapsed();
    }

protected:

    typedef std::chrono::high_resolution_clock              Clock;
    typedef std::chrono::high_resolution_clock::time_point  TimePoint;
    typedef std::chrono::high_resolution_clock::duration    Duration;

    TimePoint last;
    double total;

    bool paused;

    void update();

};

#endif // ACCURATETIMER_H
