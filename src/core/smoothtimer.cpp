#include "smoothtimer.h"

#include "mathhelper.h"
#include "commondef.h"
#include "debug.h"

using namespace std::chrono;

#define MAX_TIMEFIX_RECORD 3
#define MAX_TIMEFIX_TOLERATION 0.030
#define MIN_TIMEFIX_STEP (3e-7)

SmoothTimer::SmoothTimer()
{
    setSmoothParam(60, 0.5);
}

void SmoothTimer::setSmoothParam(double halfTime, double decaySlope)
{
    ksum = -1.0/halfTime;
    dslp = decaySlope;
    reset();
}

void SmoothTimer::reset()
{
    timer.restart();
    realStamp = timeReal();
    timeUnfix = 0;
    resetTimeRecord();
    insertTimeRecord(realStamp, rawStamp);
}

void SmoothTimer::pause()
{
    timer.pause();
}

void SmoothTimer::resume()
{
    timer.resume();
}

double SmoothTimer::getDeltaTime()
{
    double newSec=timeReal();
    double result=newSec-realStamp;
    realStamp=newSec;
    resetTimeRecord();
    return result;
}

double SmoothTimer::getDeltaTimeFixed(double deltaSec)
{
    timeUnfix += deltaSec;
    double newReal = timeReal();
    double newRaw = rawStamp+timeUnfix;

    // not insert additional records with timeUnfix<MIN_TIMEFIX_STEP
    if(record.size()==0 || newReal-realStamp>MIN_TIMEFIX_STEP)
    {
        // here we insert a new record
        insertTimeRecord(newReal, newRaw);
        realStamp = newReal;
        rawStamp = newRaw;
        timeUnfix = 0;
    }

    // then calculate fixed time
    double newFix=getTimeFixed(newReal);
    double result=newFix-fixStamp;

    // no go back in time
    if(result<0)
        return 0.0;

    fixStamp=newFix;

    /*
    if(fabs(unfixStamp-fixStamp)>MAX_TIMEFIX_TOLERATION)
    {
        reset();
        getTimeFixed(realStamp);
    }
    */

    //cout<<(fixStamp>rawStamp?' ':'-')<<fabs(fixStamp-rawStamp)<<endl;

    return result;

}

double SmoothTimer::timeReal()
{
    return timer.elapsed();
}

void SmoothTimer::resetTimeRecord()
{
    record.clear();
    rawStamp=fixStamp=realStamp;
    sumReal=sumRaw=sumNum=0;
    slopeSum=slopeNum=1;
}

void SmoothTimer::insertTimeRecord(double real, double raw)
{
    record.emplace_back(real, raw);
    if(record.size()>MAX_TIMEFIX_RECORD)
        record.pop_front();

    double dec = pow(2, ksum*(real-realStamp));
    decaySum(sumReal, real, dec);
    decaySum(sumRaw, raw, dec);
    decaySum(sumNum, 1, dec);

    if(record.size()>1)
    {
        double slope = (raw-sumRaw/sumNum)/(real-sumReal/sumNum);
        if(slope>10)slope=10;
        if(slope<0)slope=0;
        decaySum(slopeSum, slope, dslp);
        decaySum(slopeNum, 1, dslp);
    }

}

double SmoothTimer::getTimeFixed(double real)
{
    return sumRaw/sumNum + (record.size()>1 ?
               slopeSum/slopeNum*(real-sumReal/sumNum) : 0);
}

