#pragma once

#include <iostream>
#include <lev/math/vec2.h>

namespace lev
{
	template <typename T>
	struct Rect
	{
		T x;
		T y;

		union
		{
			struct
			{
				T w;
				T h;
			};

			struct
			{
				T width;
				T height;
			};
		};

		Rect();
		Rect(T w, T h);
		Rect(T x, T y, T w, T h);

		static const Rect& zero();
		static const Rect& one();

		bool contains(const Vec2& other) const;
		bool intersects(const Rect& other) const;

		Vec2 position() const;
		Vec2 size() const;

		T left() const;
		T right() const;
		T top() const;
		T bottom() const;

		Vec2 top_left() const;
		Vec2 top_right() const;
		Vec2 bottom_left() const;
		Vec2 bottom_right() const;

		friend std::ostream& operator << (std::ostream& os, const Rect& r)
		{
			std::cout << "{ " << r.x << ", " << r.y << ", " << r.w << ", " << r.h << " }" << std::endl;
			return os;
		}
	};

	using RectF = Rect<float>;
	using RectI = Rect<int>;

	template <typename T>
	Rect<T>::Rect()
		: x(0)
		, y(0)
		, w(0)
		, h(0)
	{
	}

	template <typename T>
	Rect<T>::Rect(T w, T h)
		: x(0)
		, y(0)
		, w(w)
		, h(h)
	{
	}

	template <typename T>
	Rect<T>::Rect(T x, T y, T w, T h)
		: x(x)
		, y(y)
		, w(w)
		, h(h)
	{
	}

	template <typename T>
	bool Rect<T>::contains(const Vec2& other) const
	{
		return (
			this->left() < other.x &&
			this->right() > other.x &&
			this->top() < other.y &&
			this->bottom() > other.y
		);
	}

	template <typename T>
	bool Rect<T>::intersects(const Rect<T>& other) const
	{
		return (
			this->left() < other.right() &&
			this->right() > other.left() &&
			this->top() < other.bottom() &&
			this->bottom() > other.top()
		);
	}

	template <typename T> Vec2 Rect<T>::position() const { return Vec2(x, y); }
	template <typename T> Vec2 Rect<T>::size()     const { return Vec2(w, h); }

	template <typename T> T Rect<T>::left()   const { return x;     }
	template <typename T> T Rect<T>::right()  const { return x + w; }
	template <typename T> T Rect<T>::top()    const { return y;     }
	template <typename T> T Rect<T>::bottom() const { return y + h; }

	template <typename T> Vec2 Rect<T>::top_left()     const { return Vec2(left(),  top());    }
	template <typename T> Vec2 Rect<T>::top_right()    const { return Vec2(right(), top());    }
	template <typename T> Vec2 Rect<T>::bottom_left()  const { return Vec2(left(),  bottom()); }
	template <typename T> Vec2 Rect<T>::bottom_right() const { return Vec2(right(), bottom()); }

	template <typename T> const Rect<T>& Rect<T>::zero() { static const Rect ZERO = Rect(0, 0, 0, 0); return ZERO; }
	template <typename T> const Rect<T>& Rect<T>::one()  { static const Rect ONE  = Rect(0, 0, 1, 1); return ONE;  }
}
