#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <cmath>

#define M_INFS 1e-7
#define M_PI 3.14159265258979323846264

inline double sind(double x)
{
    return sin(x*M_PI/180);
}

inline double cosd(double x)
{
    return cos(x*M_PI/180);
}

inline double pow2(double x)
{
    return x*x;
}

inline double dist(double x, double y)
{
    return sqrt(pow2(x)+pow2(y));
}

inline double rad(double x)
{
    return x*M_PI/180;
}

inline double deg(double x)
{
    return x*180/M_PI;
}

template <typename T>
inline T& min(T& x, T& y)
{
    return x<y?x:y;
}

template <typename T>
inline T& max(T& x, T& y)
{
    return x>y?x:y;
}

#endif // MATHHELPER_H
