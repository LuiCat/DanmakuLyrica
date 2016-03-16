#ifndef NOTELIST_H
#define NOTELIST_H

#include "entitylist.h"
#include "notedef.h"

#include "notejudge.h"
#include "noteentity.h"

#include <list>
using namespace std;

class NoteList : public EntityList<Note>
{
public:

    NoteList();

    JudgeResult judgeSingleNote(double timeSec);




protected:

    NoteJudge noteJudge;

    int lastJudgedId;

};

#endif // NOTELIST_H
