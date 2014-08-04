#ifndef NOTELIST_H
#define NOTELIST_H

#include "entitylist.h"
#include "notedef.h"

#include "notejudge.h"
#include "noteentity.h"

class NoteList : public EntityList
{
public:

    NoteList();

    JudgeResult judgeSingleNote(const MapState* state);

    void pushNote(const NoteEntity& note);

protected:

    NoteJudge noteJudge;

};

#endif // NOTELIST_H
