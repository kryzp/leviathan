#pragma once

#include <lev/math/vec2.h>
#include <lev/math/colour.h>

namespace lev
{
	struct Colour;

	enum class LogType;

	struct AppConfig
	{
		const char* name = nullptr;
		int width = 1280;
		int height = 720;
		int target_framerate = 60;
		bool resizable = false;
		bool vsync = true;

		void (*on_init)() = nullptr;
		void (*on_destroy)() = nullptr;
		void (*on_update)() = nullptr;
		void (*on_render)() = nullptr;
		void (*on_log)(const char*, LogType) = nullptr;
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
		Size2 window_size();

		int draw_width();
		int draw_height();
		Size2 draw_size();

		float fps();
		void clear(const Colour& colour = Colour::empty());
	}
}
