#ifndef ACCURATETIMER_H
#define ACCURATETIMER_H

#include <chrono>

template <typename Clock = std::chrono::high_resolution_clock>
class AccurateTimer
{
public:

    AccurateTimer()
    {
        restart();
    }

    void restart()
    {
        total = 0.0;
        last = Clock::now();
    }

    void pause()
    {
        if(paused)
            return;
        update();
        paused=true;
    }

    void resume()
    {
        if(paused)
        {
            last = Clock::now();
            paused=false;
        }
    }

    double elapsed()
    {
        if(!paused)
            update();
        return total;
    }

    inline double operator()()
    {
        return elapsed();
    }

protected:

    using duration_double = std::chrono::duration<double>;

    typedef typename Clock::time_point  TimePoint;
    typedef typename Clock::duration    Duration;

    TimePoint last;
    double total;

    bool paused;

    void update()
    {
        TimePoint now = Clock::now();
        total += std::chrono::duration_cast<duration_double>(now-last).count()*1e-9;
        last = now;
    }

};

#endif // ACCURATETIMER_H
