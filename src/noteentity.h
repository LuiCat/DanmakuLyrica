#ifndef NOTEENTITY_H
#define NOTEENTITY_H

#include "entity.h"

#include "notedef.h"
#include "notejudge.h"

class NoteEntity : public Entity
{
public:

    NoteEntity(double _hitTime, double _hitBeat);

    void setJudgeTime(double _hitTime, double _hitBeat);

    double getTimeOffset(const MapState* state) const;

    void setJudgeResult(const MapState *state, JudgeResult result);
    bool isJudged() const;

protected:

    double hitTime;
    double hitBeat;

    double judgeTime;

    JudgeResult judgeResult;

    virtual void onTick();
    virtual void onRender();

};

#endif // NOTEENTITY_H
