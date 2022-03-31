#pragma once

#include <lev/core/util.h>

namespace lev
{
	class Timer
	{
	public:
		Timer();

		void start();
		void stop();
		void pause();
		void unpause();

		void restart();

		u64 milliseconds() const;
		u64 seconds() const;

		bool started() const;
		bool paused() const;

	private:
		u64 m_start_ticks;
		bool m_started;

		u64 m_paused_ticks;
		bool m_paused;
	};
}
