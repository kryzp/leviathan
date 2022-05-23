#pragma once

#include <lev/math/rand.h>

namespace lv
{
	struct Range
	{
		float min;
		float max;

		Range()
		{
		}

		Range(float min, float max)
			: min(min)
			, max(max)
		{
		}

		float delta() const
		{
			return max - min;
		}

		float random() const
		{
			return Rand<std::mt19937>::inst()->real(min, max);
		}
	};
}
