#include "notejudge.h"

NoteJudge::NoteJudge()
    :rangeBad(0.210)
    ,rangeGood(0.150)
    ,rangeGreat(0.075)
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
