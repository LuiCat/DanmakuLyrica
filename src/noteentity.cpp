#include "noteentity.h"

#include "soundregistry.h"

#include "mathhelper.h"

TexturePiece Note::texNote("data/image/note/note.png");

double Note::moveSpeed = 110;

Note::Note(double _hitTime, double _hitBeat)
    :noteType(none)
    ,scrollSpeed(1.0)
    ,judgeTime(-1e7)
    ,judgeResult(Judge_Miss)
{
    setForceFacing(false);
    setJudgeTime(_hitTime, _hitBeat);
}

Note::Note(const NoteInfo& info, double forwardBeat)
    :noteType(info.noteType)
    ,scrollSpeed(info.hs)
    ,judgeTime(-1e7)
    ,judgeResult(Judge_Miss)
{
    setForceFacing(false);
    setJudgeTime(info.offsetSec, info.offsetBeat+forwardBeat);
}

void Note::setJudgeTime(double _hitTime, double _hitBeat)
{
    hitTime=_hitTime;
    hitBeat=_hitBeat;
    setPosition(_hitBeat*moveSpeed*scrollSpeed, 0);
    setSpeedRotation(moveSpeed*scrollSpeed, rad(-90));
    singleTick(_hitBeat);
}

double Note::getJudgeBeat() const
{
    return hitBeat;
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
    this->destroy();
}

bool Note::isJudged() const
{
    return judgeResult!=Judge_Miss;
}

void Note::onDestroy()
{
    setSpeed(0);
    singleTick(0.5);
}

void Note::onTick()
{
    //if(!isJudged() && hitTime<getTimeSec()+0.01)
        //setJudgeResult(0, great);
    if(isDestroyed)
    {
        setDead();
    }
    else
    {
        setSpeed(speed * 0.3);
    }
}

void Note::onRender()
{
    //d3d.setColor(judgeResult==miss?0xffffff:judgeResult==bad?0xff0000:judgeResult==good?0xffff00:0x00ff00);

    if(isDestroyed)
    {
        d2d.setAlpha(1-2*(timeSec-destroyTime));
        //if(timeSec > destroyTime + 0.5)
            //setDead();
    }
    else if(getX()<0)
    {
        d2d.setAlpha(1+getX()*0.02);
    }

    texNote.vertice(0, 0.5, 90, 100);
}
