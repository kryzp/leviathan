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
		int target_fps = 60;
		bool resizable = false;
		bool vsync = true;

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
			App* instance = nullptr;
			if (!instance) { instance = new App(); }
			return instance;
		}

		void start(const Config& cfg);

		float fps();
		void clear(const Colour& colour = Colour::empty());

		void exit();
		bool is_running();
		const Config& config();

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
