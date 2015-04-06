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

Note::Note(const NoteInfo& info, double forwardBeat)
    :scrollSpeed(info.hs)
    ,noteType(info.noteType)
    ,judgeTime(-1e7)
    ,judgeResult(miss)
{
    setForceFacing(false);
    setJudgeTime(info.offsetSec, info.offsetBeat+forwardBeat);
}

void Note::setJudgeTime(double _hitTime, double _hitBeat)
{
    hitTime=_hitTime;
    hitBeat=_hitBeat;
    setPosition(_hitBeat*120, 0);
    setSpeedRotation(120.0, rad(-90));
}

void Note::setScrollSpeed(double hs)
{
    scrollSpeed=hs;
}

void Note::setNoteType(NoteType type)
{
    noteType=type;
}

double Note::getTimeOffset(double timeSec) const
{
    return hitTime-timeSec;
}

void Note::setJudgeResult(double timeSec, JudgeResult result)
{
    judgeResult=result;
    judgeTime=(timeSec>0?timeSec:hitTime);
}

bool Note::isJudged() const
{
    return judgeResult!=miss;
}

void Note::onTick()
{
    //if(!isJudged() && hitTime<getTimeSec()+0.01)
        //setJudgeResult(0, great);
}

void Note::onRender()
{
    d3d.setColor(judgeResult==miss?0xffffff:judgeResult==bad?0xff0000:judgeResult==good?0xffff00:0x00ff00);
    d3d.pushVertex( 0.5,  10, 0.0, 0.0);
    d3d.pushVertex(-0.5,  10, 1.0, 0.0);
    d3d.pushVertex(-0.5, -10, 1.0, 1.0);
    d3d.pushVertex( 0.5, -10, 0.0, 1.0);
}
