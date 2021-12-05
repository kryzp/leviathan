#pragma once

namespace Lev
{
	struct AppConfig;

	namespace Platform
	{
		bool init(const AppConfig* cfg);
		void destroy();

		void prepare();
		void update();
		void present();

		int window_width();
		int window_height();

		int draw_width();
		int draw_height();
	}
}
