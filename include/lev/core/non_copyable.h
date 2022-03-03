#pragma once

namespace Lev
{
	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable& operator = (const NonCopyable&) = delete;
		NonCopyable(const NonCopyable&) = delete;
	};
}
