#include "notescene.h"

NoteScene::NoteScene()
{
}

void NoteScene::load()
{
    //Note::texNote.load("data/image/note/note.png");

    panelBg.load("data/image/note/panel_bg.png");
    panelIcon.load("data/image/note/panel_icon.png");

    track.load("data/image/note/track.png", 0.25, 0, 0.75, 1);
    trackLight.load("data/image/note/track_light.png");

    title.load("data/image/note/title.png");

    comboNum.loadImage("data/image/number/num.png", 3, 4);
    comboNum.setAlignment(NumberSprite::AlignCenter);
    comboNum.setCharSize(80, 60, 0.55);
    comboNum.setNumber(0);

    scoreNum.loadImage("data/image/number/num.png", 3, 4);
    scoreNum.setPosition(390, -57);
    scoreNum.setAlignment(NumberSprite::AlignRight);
    scoreNum.setCharSize(40, 30, 0.6);
    scoreNum.setFormat("%06d");
    scoreNum.setNumber(0);

    combo = 0;

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

    if(score->isUpdated())
        scoreNum.setNumber(score->getScore());

    beatTime += deltaTime;
}

void NoteScene::render()
{
    double scale;

    d2d.pushMatrix();
    d2d.translate(400, 572);

    track.vertice(0.5, 0.5, 800, 80);

    d2d.pushMatrix();
    noteList.renderAll();
    d2d.scale(-1, 1);
    noteList.renderAll();
    d2d.popMatrix();

    d2d.pushMatrix();
    d2d.setAlpha(0.0 + 0.8 * exp(-10 * beatTime.time));
    trackLight.vertice(0, 0.5, 240, 80);
    d2d.scale(-1, 1);
    trackLight.vertice(0, 0.5, 240, 80);
    d2d.popMatrix();

    panelBg.vertice(0.5, 0.5, 160);

    d2d.pushMatrix();
    d2d.translate(0, -5);
    d2d.setAlpha(0.3 + 0.5 * exp(-5 * beatTime.time));
    panelIcon.vertice(0.5, 0.5, 140);
    d2d.popMatrix();

    d2d.pushMatrix();
    scale = 1+0.15*exp(-10*beatTime.time);
    d2d.translate(0, -10);
    d2d.scale(1.0, scale);
    comboNum.render();
    d2d.popMatrix();

    d2d.pushMatrix();
    d2d.translate(-400, -55);
    title.vertice(0, 0.5, 400, 30);
    d2d.popMatrix();

    scoreNum.render();

    d2d.popMatrix();
}

void NoteScene::setNoteMap(NoteMap* m)
{
    noteMap=m;
}

JudgeResult NoteScene::judgeSingleNote(double timeSec)
{
    beatTime = BeatTimeVec::zero;
    auto res = noteList.judgeSingleNote(timeSec);
    if(res>Judge_Miss)
        combo+=1;
    else if(res==Judge_Miss)
        combo=0;
    score->setCombo(combo);
    comboNum.setNumber(combo);
    return res;
}

void NoteScene::breakCombo()
{
    combo = 0;
    comboNum.setNumber(combo);
}

Note* NoteScene::getNextPendingNote()
{
    return nullptr;
}

void NoteScene::setScoreObj(Score* score)
{
    this->score = score;
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
