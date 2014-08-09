#ifndef TIMELINE_H
#define TIMELINE_H

#define MAX_TIMEFIX_RECORD 3
#define MAX_TIMEFIX_TOLERATION 0.016

class TimeLine
{
public:

    TimeLine();

    void reset();

    double getDeltaTime();
    double getDeltaTimeFixed(double deltaSec);

protected:

    double getCurrentTime();

    void resetTimeRecord();
    void insertTimeRecord(double stamp, double unfix);

    double getTimeFixed(double stamp);

private:

    double beginTime;

    double timeStamp;
    double unfixStamp;
    double fixStamp;

    double errorSum;

    double timeRecord[MAX_TIMEFIX_RECORD];
    double unfixRecord[MAX_TIMEFIX_RECORD];

    int fixNum;


};

#endif // TIMELINE_H
