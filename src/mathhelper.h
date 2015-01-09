#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <cmath>

#define M_DINFS 1e-7
#define M_DINF ((double)0x7FF0000000000000LL)
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
inline const T& min(const T& x, const T& y)
{
    return x<y?x:y;
}

template <typename T>
inline const T& max(const T& x, const T& y)
{
    return x>y?x:y;
}

template <typename T>
inline const T tabs(const T& x)
{
    return x<0?-x:x;
}

#endif // MATHHELPER_H
