#include <lev/math/polygon.h>
#include <lev/math/calc.h>
#include <lev/math/vec2.h>

using namespace Lev;

Polygon::Polygon()
{
}

Polygon::Polygon(const Vector<Vec2>& vs)
	: vertices(vs)
{
}

bool Polygon::axis_overlaps(const Polygon& a, const Polygon& b, const Vec2& axis, float* amount)
{
	float min_a = 0.0f, max_a = 0.0f;
	float min_b = 0.0f, max_b = 0.0f;

	a.project(axis, &min_a, &max_a);
	b.project(axis, &min_b, &max_b);

	if (Calc::abs(min_b - max_a) < Calc::abs(max_b - min_a))
		(*amount) = min_b - max_a;
	else
		(*amount) = max_b - min_a;

	return (
		(min_a < max_b) &&
		(max_a > min_b)
	);
}

void Polygon::project(const Vec2& axis, float* min, float* max) const
{
	float dot = Vec2::dot(vertices[0], axis);
	(*min) = dot;
	(*max) = dot;

	for (int i = 1; i < vertices.size(); i++)
	{
		dot = Vec2::dot(vertices[i], axis);
		(*min) = Calc::min(dot, *min);
		(*max) = Calc::max(dot, *max);
	}
}

void Polygon::foreach_point(const std::function<void(int, const Vec2&, const Vec2&)>& on_vertex)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		Vec2 curr = vertices[i];
		Vec2 next = vertices[((i+1) >= vertices.size()) ? 0 : (i+1)];
		on_vertex(i, curr, next);
	}
}
