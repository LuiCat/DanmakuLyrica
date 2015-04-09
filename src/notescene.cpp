#include "notescene.h"

NoteScene::NoteScene()
{
}

void NoteScene::init()
{

}

void NoteScene::cleanup()
{
    noteList.clearAll();
}

void NoteScene::update(double deltaSec)
{
    noteList.updateAll(deltaSec);
}

void NoteScene::render()
{
    noteList.renderAll();
}

void NoteScene::setNoteMap(NoteMap* m)
{
    noteMap=m;
}

JudgeResult NoteScene::judgeSingleNote(double timeSec)
{
    return noteList.judgeSingleNote(timeSec);
}

void NoteScene::reloadNotes()
{
    noteList.clearAll();
    if(!noteMap)return;
    list<NoteInfo> tempList;
    noteMap->getNoteInfo(tempList);
    double forward=noteMap->getBeginState().calcBeatOffset(noteMap->getBeginState().timeOffset);
    for(NoteInfo& x : tempList)
    {
        noteList.newEntity<Note>(x, forward);
    }
}
