#include "attacktrail.h"
#include "debug.h"

AttackTrail::AttackTrail(int x, int y, int targetX, int targetY)
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

    d3d.setColor(0xFFFFFF);
    d3d.setAlpha(0.5-timeSec);

    double pos;
    for(pos = timeSec; pos+0.005<0.5; pos+=0.005)
    {
        double npos = pos+0.02;
        d3d.vertex(f(pos)*(w-2.5),  pos*len*2,  0, 0);
        d3d.vertex(f(npos)*(w-2.5), npos*len*2, 0, 0);
        d3d.vertex(f(npos)*(w+2.5), npos*len*2, 0, 0);
        d3d.vertex(f(pos)*(w+2.5),  pos*len*2,  0, 0);
    }

    d3d.vertex(f(pos)*(w-2.5), pos*len*2, 0, 0);
    d3d.vertex(0, len, 0, 0);
    d3d.vertex(0, len, 0, 0);
    d3d.vertex(f(pos)*(w+2.5), pos*len*2, 0, 0);

}
