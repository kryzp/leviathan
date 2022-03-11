#pragma once

namespace lev
{
	struct Colour;

	enum class LogType;

	struct AppConfig
	{
		const char* name;
		int width;
		int height;
		int target_framerate;
		bool resizable;
		bool vsync;

		void (*on_init)();
		void (*on_destroy)();
		void (*on_update)();
		void (*on_render)();
		void (*on_log)(const char*, LogType);

		AppConfig();
	};

	namespace App
	{
		void start(const AppConfig& cfg);
		bool init();
		void destroy();
		void run();

		void exit();
		bool is_running();
		const AppConfig& config();

		int window_width();
		int window_height();

		int draw_width();
		int draw_height();

		float fps();
		void clear(const Colour& colour);
	}
}
