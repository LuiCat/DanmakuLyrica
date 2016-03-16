#include "score.h"

Score::Score()
{
    combo = 0;
    score = 0;
    multiplier = 1;
    updated = true;
}

void Score::setCombo(int x)
{
    combo = x;
    if(x<50)
        multiplier = 1+x/10;
    else if(x<100)
        multiplier = 6;
    else
        multiplier = 8;
}

void Score::addScore(int x)
{
    score+=x*multiplier;
    updated=true;
}

void Score::addRawScore(int x)
{
    score+=x;
    updated=true;
}

bool Score::isUpdated()
{
    if(updated)
    {
        updated = false;
        return true;
    }
    return false;
}

