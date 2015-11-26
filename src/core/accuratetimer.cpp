#include "accuratetimer.h"

using namespace std::chrono;

AccurateTimer::AccurateTimer()
{
    restart();
}

void AccurateTimer::restart()
{
    total = 0.0;
    last = Clock::now();
}

void AccurateTimer::pause()
{
    if(paused)
        return;
    update();
    paused=true;
}

void AccurateTimer::resume()
{
    if(paused)
    {
        last = Clock::now();
        paused=false;
    }
}

double AccurateTimer::elapsed()
{
    if(!paused)
        update();
    return total;
}

void AccurateTimer::update()
{
    TimePoint now = Clock::now();
    total += (now-last).count()*1e-9;
    last = now;
}

