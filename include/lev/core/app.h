#pragma once

namespace Lev
{
	enum class LogType;

	struct ProcessConfig
	{
		// would control exactly how much
		// ram the app would take up, whether
		// it should allocate all of it at the
		// start, etc...
	};

	struct AppConfig
	{
		const char* name;
		int width;
		int height;
		int target_framerate;
		bool resizable;
		// vsync option?

		void (*on_init)();
		void (*on_destroy)();
		void (*on_update)();
		void (*on_render)();
		void (*on_log)(const char*, LogType);

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
	}
}
