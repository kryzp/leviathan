#include <lev/math/rect.h>
#include <lev/math/vec2.h>

using namespace Lev;

Rect::Rect()
	: x(0)
	, y(0)
	, w(0)
	, h(0)
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

bool Rect::contains(const Vec2& other) const
{
	return (
		this->left() < other.x &&
		this->right() > other.x &&
		this->top() < other.y &&
		this->bottom() > other.y
	);
}

bool Rect::intersects(const Rect& other) const
{
	return (
		this->left() < other.right() &&
		this->right() > other.left() &&
		this->top() < other.bottom() &&
		this->bottom() > other.top()
	);
}

float Rect::left()         const { return x; }
float Rect::right()        const { return x + w; }
float Rect::top()          const { return y; }
float Rect::bottom()       const { return y + h; }
Vec2  Rect::top_left()     const { return Vec2(left(),  top()); }
Vec2  Rect::top_right()    const { return Vec2(right(), top()); }
Vec2  Rect::bottom_left()  const { return Vec2(left(),  bottom()); }
Vec2  Rect::bottom_right() const { return Vec2(right(), bottom()); }

const Rect Rect::ZERO  = Rect(0, 0, 0, 0);
const Rect Rect::ONE   = Rect(0, 0, 1, 1);
