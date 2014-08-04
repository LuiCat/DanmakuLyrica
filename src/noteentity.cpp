#include "noteentity.h"

Note::Note(double _hitTime, double _hitBeat)
    :scrollSpeed(1.0)
    ,noteType(none)
    ,judgeTime(-1e7)
    ,judgeResult(miss)
{
    setJudgeTime(_hitTime, _hitBeat);
}

void Note::setJudgeTime(double _hitTime, double _hitBeat)
{
    hitTime=_hitTime;
    hitBeat=_hitBeat;
}

void Note::setScrollSpeed(double hs)
{
    scrollSpeed=hs;
}

void Note::setNoteType(NoteType type)
{
    noteType=type;
}

double Note::getTimeOffset(const MapState *state) const
{
    double currentTime=0.0;
    if(state)currentTime=state->timeOffset;
    return hitTime-currentTime;
}

void Note::setJudgeResult(const MapState *state, JudgeResult result)
{
    judgeResult=result;
    judgeTime=state->timeOffset;
}

bool Note::isJudged() const
{
    return judgeResult!=miss;
}

void Note::onTick()
{

}

void Note::onRender()
{

}
