#include "notescene.h"

NoteScene::NoteScene()
{
}

void NoteScene::init()
{
    noteMap.loadTjaFile("data/test.lrc");
}

void NoteScene::cleanup()
{

}

void NoteScene::update(double deltaSec)
{
    noteList.updateAll(deltaSec);
}

void NoteScene::render()
{
    noteList.renderAll();
}
