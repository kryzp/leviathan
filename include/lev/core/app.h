#pragma once

namespace Lev
{
	enum class RendererType;

	struct Config
	{
		const char* name;
		int width;
		int height;
		int target_framerate;
		bool resizable;

		void (*on_startup)();
		void (*on_update)();
		void (*on_render)();
		void (*on_shutdown)();
		void (*on_exit_request)();

		Config();
	};

	namespace App
	{
		void start(const Config* cfg);
		bool init();
		void destroy();
		void run();

		void exit();
		bool is_running();
		const Config* config();

		int window_width();
		int window_height();
		int draw_width();
		int draw_height();

		RendererType renderer_type();
	}
}
