#pragma once

namespace lev
{
	struct AppConfig;

	namespace System
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

		void* gl_context_create();
		void gl_context_make_current(void* context);
		void gl_context_destroy(void* context);

		bool gl_load_glad_loader();
	}
}
