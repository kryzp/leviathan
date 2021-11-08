#include <lev/math/rect.h>
#include <lev/math/vec2.h>

using namespace Lev;

Rect::Rect()
{
}

Rect::Rect(float w, float h)
	: x(0)
	, y(0)
	, w(w)
	, h(h)
{
}

Rect::Rect(float x, float y, float w, float h)
	: x(x)
	, y(y)
	, w(w)
	, h(h)
{
}

float Rect::left() const
{
	return x;
}

float Rect::right() const
{
	return x + w;
}

float Rect::top() const
{
	return y;
}

float Rect::bottom() const
{
	return y + h;
}

bool Rect::intersects(const Vec2& other)
{
	return (
		this->left() < other.x &&
		this->right() > other.x &&
		this->top() < other.y &&
		this->bottom() > other.y
	);
}

bool Rect::intersects(const Rect& other)
{
	return (
		this->left() < other.right() &&
		this->right() > other.left() &&
		this->top() < other.bottom() &&
		this->bottom() > other.top()
	);
}
