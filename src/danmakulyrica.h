#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

#include "notescene.h"
#include "sounds.h"

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

};

#endif // DANMAKULYRICA_H
