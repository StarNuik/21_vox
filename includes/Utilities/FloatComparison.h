
#pragma once

inline bool FloatLessThan(const float& a, const float& b)
{
    const float eps = 0.00001f;
    return (b - a - eps > 0.f);
}

inline bool FloatLessThan(const float& a, const float& b, float& eps)
{
    return (b - a - eps > 0.f);
}

inline bool FloatMoreThan(const float& a, const float& b)
{
    const float eps = 0.00001f;
    return (b - a - eps < 0.f);
}

inline bool FloatMoreThan(const float& a, const float& b, float& eps)
{
    return (b - a - eps < 0.f);
}

inline bool FloatEqual(const float& a, const float& b)
{
    const float eps = 0.00001f;
    return std::abs(a - b) < eps;
}

inline bool FloatEqual(const float& a, const float& b, const float& eps)
{
    return std::abs(a - b) < eps;
}
