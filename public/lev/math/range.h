#pragma once

#include <lev/math/random.h>

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
			return Random<std::mt19937>::inst()->real(min, max);
		}
	};
}
