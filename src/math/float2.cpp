#include <lev/math/float2.h>
#include <lev/math/vec2.h>

using namespace Lev;

Float2::Float2()
{
}

Float2::Float2(float x)
	: x(x)
	, y(x)
{
}

Float2::Float2(float x, float y)
	: x(x)
	, y(y)
{
}

Vec2 Float2::to_vec2() const
{
	return Vec2(x, y);
}
