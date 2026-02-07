#include "Collision.h"

bool IsHitCircle(const Circle& a, const Circle& b)
{
    Vec2 diff = a.pos - b.pos;
    float distSq = diff.LengthSq();
    float radiusSum = a.radius + b.radius;

    return distSq <= radiusSum * radiusSum;
}

bool IsOutOfScreen(const Circle& c, int screenWidth, int screenHeight)
{
    if (c.pos.x - c.radius < 0) return true;
    if (c.pos.x + c.radius > screenWidth) return true;
    if (c.pos.y - c.radius < 0) return true;
    if (c.pos.y + c.radius > screenHeight) return true;

    return false;
}
