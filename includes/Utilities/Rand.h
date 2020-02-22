#pragma once
#include <iostream>

inline int IntRand(int min, int max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}

inline float FloatRand(float min, float max)
{
    return min + (rand() / (RAND_MAX / (max - min)));  
}

inline double DoubleRand(double min, double max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}