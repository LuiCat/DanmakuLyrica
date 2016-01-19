#include "notejudge.h"

NoteJudge::NoteJudge()
    :rangeBad(0.240)
    ,rangeGood(0.150)
    ,rangeGreat(0.045)
{
}

JudgeResult NoteJudge::judgeNote(double offsetSec)
{
    if(offsetSec<0)
        offsetSec=-offsetSec;
    if(offsetSec<rangeGreat)
        return Judge_Great;
    if(offsetSec<rangeGood)
        return Judge_Cool;
    //if(offsetSec<rangeBad)
        //return bad;
    return Judge_Miss;
}
