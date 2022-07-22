#pragma once

#include <lev/core/util.h>

#include <lev/audio/sound.h>

namespace lev::bknd
{
	struct AudioProperties
	{
		int temp;
	};

	class Audio : public NonCopyable, public NonMovable
	{
	public:
		Audio() = default;
		virtual ~Audio() = default;

		static Audio* inst();

		virtual AudioProperties properties() = 0;

		virtual bool init() = 0;
		virtual void destroy() = 0;
		virtual void update() = 0;

		virtual Ref<Sound> create_sound(const char* path) = 0;
	};
}
