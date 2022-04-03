#pragma once

#include <lev/math/vec2.h>
#include <lev/math/colour.h>

#include <functional>

namespace lev
{
	struct Colour;

	enum LogType;

	struct Config
	{
		const char* name = nullptr;
		int width = 1280;
		int height = 720;
		int target_tps = 60;
		bool resizable = false;
		bool vsync = true;

		std::function<void(void)> on_init = nullptr;
		std::function<void(void)> on_destroy = nullptr;
		std::function<void(void)> on_update = nullptr;
		std::function<void(void)> on_render = nullptr;
		std::function<void(const char*, LogType)> on_log = nullptr;
	};

	namespace App
	{
		void start(const Config& cfg);
		bool init();
		void destroy();
		void run();

		void exit();
		bool is_running();
		const Config& config();

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
