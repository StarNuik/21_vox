
#pragma once

inline bool FloatLessThan(const float& a, const float& b)
{
    float eps = 0.00001f;
    if (a < 0.f || b < 0.f)
    {
        eps = -eps;
        return a + b < eps ? true : false;
    }
    return (a - b) < eps ? true : false;
}

inline bool FloatLessThan(const float& a, const float& b, float& eps)
{
    if (a < 0.f || b < 0.f)
    {
        eps = -eps;
        return (a + b) < eps ? true : false;
    }
    return (a - b) < eps ? true : false;
}

inline bool FloatMoreThan(const float& a, const float& b)
{
    float eps = 0.00001f;
    if (a < 0.f || b < 0.f)
    {
        eps = -eps;
        return a + b > eps ? true : false;
    }
    return (a - b) > eps ? true : false;
}

inline bool FloatMoreThan(const float& a, const float& b, float& eps)
{
    if (a < 0.f || b < 0.f)
    {
        eps = -eps;
        return a + b > eps ? true : false;
    }
    return (a - b) > eps ? true : false;
}

inline bool FloatEqual(const float& a, const float& b)
{
    return a - b == 0.f ? true : false;
}
