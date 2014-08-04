#ifndef NOTELIST_H
#define NOTELIST_H

#include "entitylist.h"
#include "notedef.h"

#include "notejudge.h"
#include "noteentity.h"

#include <list>
using namespace std;

class NoteList : public EntityList
{
public:

    NoteList();

    JudgeResult judgeSingleNote(const MapState* state);

    void pushNote(const Note& note);
    void pushNotes(const list<Note>& notes);

protected:

    NoteJudge noteJudge;

};

#endif // NOTELIST_H
