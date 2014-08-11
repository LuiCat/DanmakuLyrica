#include "noteentity.h"

#include "soundregistry.h"

#include "mathhelper.h"

Note::Note(double _hitTime, double _hitBeat)
    :scrollSpeed(1.0)
    ,noteType(none)
    ,judgeTime(-1e7)
    ,judgeResult(miss)
{
    setForceFacing(false);
    setJudgeTime(_hitTime, _hitBeat);
}

void Note::setJudgeTime(double _hitTime, double _hitBeat)
{
    hitTime=_hitTime;
    hitBeat=_hitBeat;
    setPosition(_hitTime*300, 0);
    setSpeedRotation(5.0, rad(-90));
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
    judgeTime=(state?state->timeOffset:hitTime);
    if(judgeResult!=miss)
        SOUND("hit0")->play(true);
}

bool Note::isJudged() const
{
    return judgeResult!=miss;
}

void Note::onTick()
{
    if(!isJudged() && hitTime<getTimeSec()+0.05)
        setJudgeResult(0, great);
}

void Note::onRender()
{
    d3d.pushVertex( 0.5,  10, 0.0, 0.0);
    d3d.pushVertex(-0.5,  10, 1.0, 0.0);
    d3d.pushVertex(-0.5, -10, 1.0, 1.0);
    d3d.pushVertex( 0.5, -10, 0.0, 1.0);
}
