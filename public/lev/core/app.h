#pragma once

#include <lev/math/vec2.h>
#include <lev/math/colour.h>

#include <functional>

namespace lev
{
	struct Colour;

	struct Config
	{
		const char* name = nullptr;
		int width = 1280;
		int height = 720;
		int target_fps = 60;
		int max_updates = 5;
		bool resizable = false;
		bool vsync = true;
		bool cursor_visible = true;

		std::function<void(void)> on_init = nullptr;
		std::function<void(void)> on_exit = nullptr;
		std::function<void(void)> on_destroy = nullptr;
		std::function<void(void)> on_update = nullptr;
		std::function<void(void)> on_render = nullptr;
		std::function<void(const char*, LogType)> on_log = nullptr;
	};

	class App
	{
	public:
		static App* inst()
		{
			static App* instance = nullptr;
			if (!instance) { instance = new App(); }
			return instance;
		}

		void start(const Config& cfg);
		void exit();
		const Config& config();

		float fps() const;
		bool is_running() const;

		static void clear(const Colour& colour = Colour::empty());

		static void show_cursor(bool toggle);
		static bool cursor_visible();

		static Vec2I window_position();
		static void window_position(int x, int y);

		static int window_width();
		static int window_height();
		static Size2 window_size();

		static int draw_width();
		static int draw_height();
		static Size2 draw_size();

	private:
		bool init();
		void run();
		void destroy();

		bool m_running;
		Config m_config;
	};
}
