#include "timeline.h"

#include "mathhelper.h"
#include "commondef.h"

#include "debug.h"

TimeLine::TimeLine()
{
    reset();
}

void TimeLine::reset()
{
    beginTime=timeGetTime()*0.001;
    timeStamp=0;
    resetTimeRecord();
}

double TimeLine::getDeltaTime()
{
    double newSec=getCurrentTime();
    double result=newSec-timeStamp;
    timeStamp=newSec;
    resetTimeRecord();
    return result;
}

double TimeLine::getDeltaTimeFixed(double deltaSec)
{
    timeStamp=getCurrentTime();
    unfixStamp+=deltaSec;

    insertTimeRecord(timeStamp, unfixStamp);

    double newFix=getTimeFixed(timeStamp);
    double result=newFix-fixStamp;
    errorSum+=unfixStamp-newFix;
    fixStamp=newFix;

    if(tabs(errorSum)>MAX_TIMEFIX_TOLERATION)
    {
        cout<<"reset "<<timeStamp<<' '<<unfixStamp<<' '<<newFix<<endl;
        result+=errorSum;
        resetTimeRecord();
        insertTimeRecord(timeStamp, unfixStamp);
    }

    return result;
}

double TimeLine::getCurrentTime()
{
    return timeGetTime()*0.001-beginTime;
}

void TimeLine::resetTimeRecord()
{
    fixNum=0;
    fixStamp=unfixStamp=timeStamp;
    errorSum=0;
}

void TimeLine::insertTimeRecord(double stamp, double unfix)
{
    if(fixNum<MAX_TIMEFIX_RECORD)++fixNum;
    int i;
    for(i=MAX_TIMEFIX_RECORD-1;i>0;--i)
        timeRecord[i]=timeRecord[i-1];
    for(i=MAX_TIMEFIX_RECORD-1;i>0;--i)
        unfixRecord[i]=unfixRecord[i-1];
    timeRecord[0]=stamp;
    unfixRecord[0]=unfix;
    if(fixNum<MAX_TIMEFIX_RECORD)
        fixNum++;
}

double TimeLine::getTimeFixed(double stamp)
{
    double timeSum=0;
    double unfixSum=0;
    int i;

    for(i=0;i<fixNum;++i)
        timeSum+=timeRecord[i];
    for(i=0;i<fixNum;++i)
        unfixSum+=unfixRecord[i];

    timeSum/=fixNum;
    unfixSum/=fixNum;

    return unfixSum+stamp-timeSum;
}

