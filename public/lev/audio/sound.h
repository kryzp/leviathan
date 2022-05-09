#pragma once

#include <lev/core/util.h>

namespace lv
{
	class Sound
	{
	public:
		Sound() = default;
		virtual ~Sound() = default;

		static Ref<Sound> create(const char* path);

		virtual void play() = 0;
	};
}
