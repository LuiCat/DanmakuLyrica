#ifndef NOTEENTITY_H
#define NOTEENTITY_H

#include "entity.h"

#include "notedef.h"
#include "notejudge.h"

#include "mapstate.h"

class Note : public Entity
{
public:

    Note(double _hitTime=0.0, double _hitBeat=0.0);
    Note(const NoteInfo& info);

    void setJudgeTime(double _hitTime, double _hitBeat);
    void setScrollSpeed(double hs);
    void setNoteType(NoteType type);

    double getTimeOffset(double timeSec) const;

    void setJudgeResult(double timeSec, JudgeResult result);
    bool isJudged() const;

protected:

    double hitTime;
    double hitBeat;

    double scrollSpeed;

    NoteType noteType;

    double judgeTime;

    JudgeResult judgeResult;

    virtual void onTick();
    virtual void onRender();

};

#endif // NOTEENTITY_H
