#include "notelist.h"



NoteList::NoteList()
{

}

JudgeResult NoteList::judgeSingleNote(const MapState *state)
{
    JudgeResult judge=miss;
    Note* note;

    auto iter=entityList.begin();
    while(judge==miss && iter!=entityList.end())
    {
        note=(Note*)*iter;
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

void NoteList::pushNote(const Note& note)
{
    entityList.push_back(new Note(note));
}
