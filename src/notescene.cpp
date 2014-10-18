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
    noteList.updateAll(deltaSec);
}

void NoteScene::render()
{
    noteList.renderAll();
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
    list<Note*> tempList;
    noteMap.getEntityNotes(tempList);
    noteList.pushEntities(tempList);
}
