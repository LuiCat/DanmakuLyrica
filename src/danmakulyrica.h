#ifndef DANMAKULYRICA_H
#define DANMAKULYRICA_H

#include "maingame.h"

class DanmakuLyrica : public MainGame
{
public:
    DanmakuLyrica();

protected:
    void mainInit();
    void mainCleanup();
    void mainUpdate();
    void mainRender();

};

#endif // DANMAKULYRICA_H
