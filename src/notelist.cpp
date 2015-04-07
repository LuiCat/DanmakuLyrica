#include "notelist.h"



NoteList::NoteList()
{

}

JudgeResult NoteList::judgeSingleNote(double timeSec)
{
    JudgeResult judge=miss;
    Note* note;

    auto iter=entityList.begin();
    while(judge==miss && iter!=entityList.end())
    {
        note=&(iter->second);
        if(!note->isJudged())
            judge=noteJudge.judgeNote(note->getTimeOffset(timeSec));
        iter++;
    }

    if(iter!=entityList.end())
    {
        note->setJudgeResult(timeSec, judge);
    }

    return judge;
}
