#include "notescene.h"

NoteScene::NoteScene()
{
}

void NoteScene::init()
{
    noteMap.loadTjaFile("data/test.lrc");
    reloadNotes();
}

void NoteScene::cleanup()
{
    noteList.clearAll();
}

void NoteScene::update(double deltaSec)
{
    //1: load new notes (selectable)
    //2: update notemap timeline, process time to next event
    //3:

    noteList.updateAll(deltaSec);
}

void NoteScene::render()
{
    noteList.renderAll();
}

double NoteScene::getBeginBeatOffset()
{
    const MapState& s=noteMap.getBeginState();
    return s.calcBeatOffset(s.timeOffset);
}

const char* NoteScene::getBgmFilename()
{
    return noteMap.getWavFilename();
}

JudgeResult NoteScene::judgeSingleNote(double timeSec)
{
    return noteList.judgeSingleNote(timeSec);
}

void NoteScene::reloadNotes()
{
    list<NoteInfo> tempList;
    noteMap.getNoteInfo(tempList);
    for(NoteInfo& x : tempList)
    {
        noteList.newEntity(x);
    }
}
