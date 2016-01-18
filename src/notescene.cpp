#include "notescene.h"

NoteScene::NoteScene()
{
}

void NoteScene::load()
{
    //Note::texNote.load("data/image/note/note.png");

    texPanelB.load("data/image/note/panel_bottom.png");
    texPanelO.load("data/image/note/panel_overlay.png");

    reloadNotes();

    beatTime = 1e7;
}

void NoteScene::unload()
{
    noteList.clearAll();
}

void NoteScene::update(rtime_t deltaTime)
{
    noteList.updateAll(deltaTime.beat);

    beatTime += deltaTime;
}

void NoteScene::render()
{
    d3d.pushMatrix();
    d3d.translate2D(400, 572);

    d3d.pushMatrix();
    double scale = 1-0.05*exp(-10*beatTime.sec);
    d3d.scale2D(scale, scale);
    texPanelB.vertice(0.5, 0.5, 160);
    d3d.popMatrix();

    d3d.pushMatrix();
    noteList.renderAll();
    d3d.scale2D(-1, 1);
    noteList.renderAll();
    d3d.popMatrix();

    texPanelO.vertice(0.5, 0.5, 160);

    d3d.popMatrix();
}

void NoteScene::setNoteMap(NoteMap* m)
{
    noteMap=m;
}

JudgeResult NoteScene::judgeSingleNote(double timeSec)
{
    beatTime = BeatTime::zero;
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
