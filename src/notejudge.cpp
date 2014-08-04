#include "notejudge.h"

NoteJudge::NoteJudge()
    :rangeBad(0.240)
    ,rangeGood(0.180)
    ,rangeGreat(0.060)
{
}

JudgeResult NoteJudge::judgeNote(double offsetSec)
{
    if(offsetSec<0)
        offsetSec=-offsetSec;
    if(offsetSec<rangeGreat)
        return great;
    if(offsetSec<rangeGood)
        return good;
    if(offsetSec<rangeBad)
        return bad;
    return miss;
}
