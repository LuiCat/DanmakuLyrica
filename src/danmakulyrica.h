#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

#include "notemap.h"

class DanmakuLyrica : public MainGame
{
public:

    DanmakuLyrica();

protected:

    Notemap notemap;

    void mainInit();
    void mainCleanup();
    void mainUpdate();
    void mainRender();

};

#endif // DANMAKULYRICA_H
