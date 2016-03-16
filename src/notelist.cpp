#include "notelist.h"



NoteList::NoteList()
{
    lastJudgedId = -1;
}

JudgeResult NoteList::judgeSingleNote(double timeSec)
{
    JudgeResult judge=Judge_Miss;
    Note* note;
    double offset;

    auto iter = entityList.find(lastJudgedId);
    if(iter == entityList.end())
        iter = entityList.begin();

    while(judge==Judge_Miss && iter!=entityList.end())
    {
        note = (iter->second.get());
        if(!note->isJudged())
        {
            offset = note->getTimeOffset(timeSec);
            if(offset > noteJudge.getJudgeWindow())
                return Judge_Miss;
            judge = noteJudge.judgeNote(offset);
            lastJudgedId = iter->first;
        }
        iter++;
    }

    if(judge!=Judge_Miss)
    {
        note->setJudgeResult(timeSec, judge);
    }

    return judge;
}
