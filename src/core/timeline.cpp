#include "timeline.h"

#include "mathhelper.h"
#include "commondef.h"
#include "debug.h"

using namespace std::chrono;

TimeLine::TimeLine()
{
    setSmoothParam(60, 0.5);
}

void TimeLine::setSmoothParam(double halfTime, double decaySlope)
{
    ksum = -1.0/halfTime;
    dslp = decaySlope;
    reset();
}

void TimeLine::reset()
{
    realStamp = timeReal();
    timeUnfix = 0;
    resetTimeRecord();
    insertTimeRecord(realStamp, rawStamp);
    timer.restart();
}

void TimeLine::pause()
{
    timer.pause();
}

void TimeLine::resume()
{
    timer.resume();
}

double TimeLine::getDeltaTime()
{
    double newSec=timeReal();
    double result=newSec-realStamp;
    realStamp=newSec;
    resetTimeRecord();
    return result;
}

double TimeLine::getDeltaTimeFixed(double deltaSec)
{
    timeUnfix += deltaSec;
    double newReal = timeReal();
    double newRaw = rawStamp+timeUnfix;

    //cout<<newReal<<endl;

    // not insert additional records with timeUnfix<MIN_TIMEFIX_STEP
    if(record.size()==0 || timeUnfix>MIN_TIMEFIX_STEP)
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

double TimeLine::timeReal()
{
    return timer.elapsed();
}

void TimeLine::resetTimeRecord()
{
    record.clear();
    rawStamp=fixStamp=realStamp;
    sumReal=sumRaw=sumNum=0;
    slopeSum=slopeNum=0;
}

void TimeLine::insertTimeRecord(double real, double raw)
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
        decaySum(slopeSum, slope, dslp);
        decaySum(slopeNum, 1, dslp);
    }

}

double TimeLine::getTimeFixed(double real)
{
    return sumRaw/sumNum + (record.size()>1 ?
               slopeSum/slopeNum*(real-sumReal/sumNum) : 0);
}

