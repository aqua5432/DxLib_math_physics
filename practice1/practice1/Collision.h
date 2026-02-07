#pragma once
#include "Math.h"

struct Circle
{
    Vec2 pos;
    float radius;
};

bool IsHitCircle(const Circle& a, const Circle& b);
bool IsOutOfScreen(const Circle& c, int screenWidth, int screenHeight);
