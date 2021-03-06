#include "attacktrail.h"
#include "debug.h"

AttackTrail::AttackTrail(double x, double y, double targetX, double targetY)
    : BulletBase(x, y, 0, 0)
{
    setTickRate(0.6);
    double dx = targetX-x;
    double dy = targetY-y;
    setRotation(atan2(dx, dy));
    len = dist(dx, dy);
    w = (rand()%10-4.5)*len*0.08;
    cout<<len<<" "<<w<<endl;
}

void AttackTrail::onTick()
{
    if(timeSec>0.5)
        setDead();
}

bool AttackTrail::isOutsideScene(double x1, double y1, double x2, double y2)
{
    return false;
}

void AttackTrail::onRender()
{
    if(timeSec>=0.5)
        return;

    auto f = [](double x)->double
    {
        x-=0.25;
        return 1-16*x*x;
    };

    d2d.setColor(0xFFFFFF);
    d2d.setAlpha((0.5-timeSec)*2);

    bool shaped=false;
    double pos;
    for(pos = timeSec; pos+0.005<0.5; pos+=0.01)
    {
        d2d.vertex(f(pos)*(w-2.5), pos*len*2, 0, 0);
        d2d.vertex(f(pos)*(w+2.5), pos*len*2, 0, 0);
        shaped=true;
    }

    if(shaped)
    {
        d2d.vertex(0, len, 0, 0);
        d2d.shape(Shape_Triangles);
    }

}
