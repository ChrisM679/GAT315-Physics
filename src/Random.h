#pragma once

#include "raylib.h"
#include <algorithm>

inline float GetRandomFloat()
{
    return GetRandomValue(0, 10000) / 10000.0f;
}

inline float GetRandomFloat(float max)
{
    return GetRandomFloat() * max;
}

inline float GetRandomFloat(float min, float max)
{
    if (min > max) std::swap(min, max);
    return min + GetRandomFloat() * (max - min);
}