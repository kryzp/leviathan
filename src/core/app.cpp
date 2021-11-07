#include <lev/core/app.h>
#include <lev/core/util.h>

#include <iostream>

#include <internal/platform.h>
#include <internal/graphics.h>
#include <internal/input.h>

using namespace LEV;

namespace
{
	bool g_running = false;
	Config g_config;
}

Config::Config()
{
	this->name = nullptr;
	this->width = 1280;
	this->height = 720;
	this->target_framerate = 60;
	this->resizable = false;

	this->on_startup = nullptr;
	this->on_shutdown = nullptr;
	this->on_update = nullptr;
	this->on_render = nullptr;
	this->on_exit_request = nullptr;
}

void App::start(const Config* cfg)
{
	LEVIATHAN_ASSERT(cfg != nullptr);

	g_config = *cfg;

	if (!init())
	{
		std::cout << "failed to initialize" << std::endl;
		return;
	}

	App::run();
	App::destroy();
}

bool App::init()
{
	if (g_config.on_startup)
		g_config.on_startup();

	if (!Platform::init(&g_config))
	{
		std::cout << "failed to initialize platform" << std::endl;
		return false;
	}

	if (!Graphics::init())
	{
		std::cout << "failed to initialize graphics" << std::endl;
		return false;
	}

	if (!Input::init())
	{
		std::cout << "failed to initialize input" << std::endl;
		return false;
	}

	g_running = true;

	return true;
}

void App::run()
{
	Platform::prepare();

	while (g_running)
	{
		Platform::frame();

		// update
		{
			Input::frame();

			if (g_config.on_update)
				g_config.on_update();
		}

		// render
		{
			Graphics::before_render();

			if (g_config.on_render)
				g_config.on_render();

			Graphics::after_render();
			Platform::present();
		}
	}
}

void App::destroy()
{
	if (g_config.on_shutdown)
		g_config.on_shutdown();

	Platform::shutdown();
	Graphics::shutdown();
	Input::shutdown();

	g_running = false;
}

void App::exit()
{
	if (g_config.on_exit_request)
		g_config.on_exit_request();

	g_running = false;
}

bool App::is_running()
{
	return g_running;
}

const Config* App::config()
{
	return &g_config;
}

int App::window_width()
{
	return Platform::window_width();
}

int App::window_height()
{
	return Platform::window_height();
}

int App::draw_width()
{
	return Platform::draw_width();
}

int App::draw_height()
{
	return Platform::draw_height();
}

RendererType App::renderer_type()
{
	return Graphics::renderer_type();
}
