#pragma once
#include <iostream>

inline int intRand(int min, int max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}

inline float floatRand(float min, float max)
{
    return min + (rand() / (RAND_MAX / (max - min)));  
}

inline double doubleRand(double min, double max)
{
    return min + (rand() / (RAND_MAX / (max - min)));
}