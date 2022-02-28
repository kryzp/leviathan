#pragma once

namespace Lev
{
	enum class TextureFilter
	{
		None,
		Point,
		Linear
	};

	enum class TextureWrap
	{
		None,
		Clamp,
		Repeat
	};

	struct TextureSampler
	{
		TextureFilter filter;
		TextureWrap wrap_x;
		TextureWrap wrap_y;

		TextureSampler()
			: filter(TextureFilter::Point)
			, wrap_x(TextureWrap::Clamp)
			, wrap_y(TextureWrap::Clamp)
		{
		}

		TextureSampler(TextureFilter filter, TextureWrap wrap_x, TextureWrap wrap_y)
			: filter(filter)
			, wrap_x(wrap_x)
			, wrap_y(wrap_y)
		{
		}

		bool operator == (const TextureSampler& other) const
		{
			return (
				(this->filter == other.filter) &&
				(this->wrap_x == other.wrap_x) &&
				(this->wrap_y == other.wrap_y)
			);
		}

		bool operator != (const TextureSampler& other) const
		{
			return !(*this == other);
		}
	};
}
