// 编码：Utf-8 without BOM
// Created by LuiCat

#ifndef _TIME_MANAGER_H_
#define _TIME_MANAGER_H_

#include "timeprovider.h"
#include "beatprovider.h"
#include "smoothtimer.h"
#include "beattime.h"

class TimeManager
{
protected:

    TimeProvider* timeProvider;
    BeatProvider* beatProvider;

    double timeElapsed;

public:

    BeatTime beatTime;

    explicit TimeManager();

    void setTimeProvider(TimeProvider* timeProvider);
    void setBeatProvider(BeatProvider* beatProvider);

    // * Updates the time.
    // Call this at beginning of the frame, and the time is calculated once for use.
    // When the beatProvider required multiple updates, this function returns true,
    // in which case you should call updateTime later to get full elapsed time;
    // or else it returns false, meaning no further update is required at this loop;
    // Specify noBreak == true ignores multiple update and get elapsed time at once.
    bool updateTime(bool noBreak);

    void seekTime(double timeSec);

};


#endif // _TIME_MANAGER_H_
