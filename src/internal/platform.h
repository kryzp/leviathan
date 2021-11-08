#pragma once

namespace Lev
{
	struct Config;

	namespace Platform
	{
		bool init(const Config* cfg);
		void shutdown();

		void prepare();
		void frame();
		void present();

		int window_width();
		int window_height();

		int draw_width();
		int draw_height();
	}
}
