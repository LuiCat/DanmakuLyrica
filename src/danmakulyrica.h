#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

#include "notescene.h"
#include "sounds.h"
#include "keybinding.h"

#include "timeline.h"

class DanmakuLyrica : public MainGame
{
public:

    DanmakuLyrica();

protected:

    void mainInit();
    void mainCleanup();
    void mainUpdate();
    void mainRender();

private:

    NoteScene noteScene;
    StreamBuffer bgm;

    TimeLine timeLine;

    KeyBinding buttonA;
    KeyBinding buttonB;

};

#endif // DANMAKULYRICA_H
