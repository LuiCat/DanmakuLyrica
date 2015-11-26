#include "notescene.h"

NoteScene::NoteScene()
{
}

void NoteScene::load()
{
    reloadNotes();
}

void NoteScene::unload()
{
    noteList.clearAll();
}

void NoteScene::update(rtime_t deltaTime)
{
    noteList.updateAll(deltaTime.beat);
}

void NoteScene::render()
{
    d3d.pushMatrix();
    d3d.translate2D(100, 300.0);

    d3d.pushMatrix();
    d3d.setColor(0x00FF00);
    d3d.vertex( 0.5,  20, 0.0, 0.0);
    d3d.vertex(-0.5,  20, 1.0, 0.0);
    d3d.vertex(-0.5, -20, 1.0, 1.0);
    d3d.vertex( 0.5, -20, 0.0, 1.0);
    d3d.popMatrix();

    noteList.renderAll();

    d3d.popMatrix();
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
