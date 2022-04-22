#pragma once

namespace lev
{
	template <typename First, typename Second>
	struct Pair
	{
		First first;
		Second second;

		Pair(const First& a, const Second& b)
			: first(a)
			, second(b)
		{
		}
	};
}
