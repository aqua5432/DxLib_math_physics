#include "Math.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float _x, float _y) : x(_x), y(_y) {}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(x - other.x, y - other.y);
}

float Vec2::LengthSq() const
{
    return x * x + y * y;
}

float Vec2::Length() const
{
    return std::sqrt(LengthSq());
}
