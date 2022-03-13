#include <lev/math/circle.h>

using namespace lev;

Circle::Circle()
	: position(Vec2::zero())
	, radius(0.0f)
{
}

Circle::Circle(float rad)
	: position(Vec2::zero())
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
	return radius * 2.0f;
}

const Circle& Circle::zero() { static const Circle ZERO = Circle(Vec2::zero(), 0.0f); return ZERO; }
const Circle& Circle::one()  { static const Circle ONE  = Circle(Vec2::zero(), 1.0f); return ONE;  }
