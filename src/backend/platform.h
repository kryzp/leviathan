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

		void* context_create();
		void context_make_current(void* context);
		void context_destroy(void* context);

		// todo: is there a way to make this support other libs like vulkan and directx?
		bool gl_load_glad_loader();
	}
}
