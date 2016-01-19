#include "notelist.h"



NoteList::NoteList()
{

}

JudgeResult NoteList::judgeSingleNote(double timeSec)
{
    JudgeResult judge=Judge_Miss;
    Note* note;

    auto iter=entityList.begin();
    while(judge==Judge_Miss && iter!=entityList.end())
    {
        note=(iter->second.get());
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
