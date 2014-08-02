#include "notelist.h"

#include "noteentity.h"


NoteList::NoteList()
{

}

JudgeResult NoteList::judgeSingleNote(const MapState *state)
{
    JudgeResult judge=miss;
    NoteEntity* note;

    auto iter=entityList.begin();
    while(judge==miss && iter!=entityList.end())
    {
        note=(NoteEntity*)&(*iter);
        if(!note->isJudged())
            judge=noteJudge.judgeNote(note->getTimeOffset(state));
        iter++;
    }

    if(iter!=entityList.end())
    {
        note->setJudgeResult(state, judge);
    }

    return judge;
}
