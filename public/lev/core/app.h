#pragma once

#include <lev/math/vec2.h>
#include <lev/math/colour.h>

#include <functional>

namespace lv
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
		std::function<void(const char*, u8)> on_log = nullptr;
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

		float fps();
		void clear(const Colour& colour = Colour::empty());

		void exit();
		bool is_running();
		const Config& config();

		void show_cursor(bool toggle);
		bool cursor_visible();

		int window_width();
		int window_height();
		Size2 window_size();

		int draw_width();
		int draw_height();
		Size2 draw_size();

	private:
		bool init();
		void run();
		void destroy();

		bool m_running;
		Config m_config;
	};
}
