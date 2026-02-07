#pragma once
#include <cmath>

struct Vec2
{
    float x;
    float y;

    Vec2();
    Vec2(float _x, float _y);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;

    float LengthSq() const;
    float Length() const;
};
