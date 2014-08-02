#include "noteentity.h"

NoteEntity::NoteEntity(double _hitTime, double _hitBeat)
    :hitTime(_hitTime)
    ,hitBeat(_hitBeat)
    ,judgeTime(-1e7)
{
}

double NoteEntity::getTimeOffset(const MapState *state) const
{
    double currentTime=0.0;
    if(state)currentTime=state->timeOffset;
    return hitTime-currentTime;
}

void NoteEntity::setJudgeResult(const MapState *state, JudgeResult result)
{
    judgeResult=result;
    judgeTime=state->timeOffset;
}

bool NoteEntity::isJudged()
{
    return judgeResult!=bad;
}

void NoteEntity::onTick()
{

}

void NoteEntity::onRender()
{

}
