#pragma once

namespace lev
{
	class NonCopyable;

	class NonMovable
	{
	public:
		NonMovable() = default;
		NonMovable& operator = (NonCopyable&&) = delete;
		NonMovable(NonCopyable&&) = delete;
	};
}
