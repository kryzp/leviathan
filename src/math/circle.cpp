#include <lev/math/circle.h>

using namespace Lev;

Circle::Circle()
	: position(Vec2::ZERO)
	, radius(0.0f)
{
}

Circle::Circle(float rad)
	: position(Vec2::ZERO)
	, radius(rad)
{
}

Circle::Circle(const Vec2& pos, float rad)
	: position(pos)
	, radius(rad)
{
}

float Circle::diameter() const
{
	return radius * 2;
}
