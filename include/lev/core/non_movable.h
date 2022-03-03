#pragma once

namespace Lev
{
	class NonCopyable;

	struct NonMovable
	{
		NonMovable() = default;
		NonMovable& operator = (NonCopyable&&) = delete;
		NonMovable(NonCopyable&&) = delete;
	};
}
