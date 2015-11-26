#ifndef POSITION_H
#define POSITION_H

class Position
{
protected:
    double x, y;
public:
    Position();
    Position(double _x, double _y);

    double getX() const;
    double getY() const;
    void setX(double value);
    void setY(double value);
    void setPosition(double _x, double _y);

    double distanceTo(const Position& another) const;

};


#endif // POSITION_H
