#pragma once

namespace Lev
{
	enum class RendererType;

	struct ProcessConfig
	{
		// ?? maybe maybe maybe ??
		// data like ram usage, process name, etc...
	};

	struct AppConfig
	{
		const char* name;
		int width;
		int height;
		int target_framerate;
		bool resizable;
		// vsync option?

		void (*on_update)();
		void (*on_render)();
		void (*on_init)();
		void (*on_destroy)();

		AppConfig();
	};

	namespace App
	{
		void start(const AppConfig* cfg);
		bool init();
		void destroy();
		void run();

		void exit();
		bool is_running();
		const AppConfig* config();

		int window_width();
		int window_height();
		int draw_width();
		int draw_height();

		RendererType renderer_type();
	}
}
