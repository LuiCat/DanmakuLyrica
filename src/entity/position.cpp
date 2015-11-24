#include "position.h"

Position::Position()
    :x(0.0)
    ,y(0.0)
{

}

Position::Position(double _x, double _y)
    :x(_x)
    ,y(_y)
{
}

double Position::getX() const
{
    return x;
}

void Position::setX(double value)
{
    x = value;
}

double Position::getY() const
{
    return y;
}

void Position::setY(double value)
{
    y = value;
}

void Position::setPosition(double _x, double _y)
{
    x=_x;
    y=_y;
}
