#include <lev/core/app.h>
#include <lev/core/util.h>

#include <iostream>

#include <backend/system.h>
#include <backend/renderer.h>
#include <backend/input.h>

using namespace Lev;

namespace
{
	bool g_running = false;
	AppConfig g_config;
}

AppConfig::AppConfig()
{
	this->name = nullptr;
	this->width = 1280;
	this->height = 720;
	this->target_framerate = 60;
	this->resizable = false;

	this->on_init = nullptr;
	this->on_destroy = nullptr;
	this->on_update = nullptr;
	this->on_render = nullptr;
	this->on_log = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////

void App::start(const AppConfig* cfg)
{
	LEV_ASSERT(cfg != nullptr);

	g_config = *cfg;

	if (!init())
	{
		Log::error("exiting...");
		return;
	}

	App::run();
	App::destroy();
}

bool App::init()
{
#ifdef LEV_DEBUG
	if (!Log::init())
	{
		// r/hmmm
		Log::error("failed to initialize logging");

		return false;
	}
#endif

	if (!System::init(&g_config))
	{
		Log::error("failed to initialize platform");
		return false;
	}

	if (!Renderer::init())
	{
		Log::error("failed to initialize rendering");
		return false;
	}

	if (!Input::init())
	{
		Log::error("failed to initialize input");
		return false;
	}

	if (g_config.on_init)
		g_config.on_init();

	g_running = true;

	return true;
}

void App::run()
{
	System::prepare();

	while (g_running)
	{
		System::update();

		// update
		{
			Input::update();
			
			if (g_config.on_update)
				g_config.on_update();
		}

		// render
		{
			Renderer::before_render();

			if (g_config.on_render)
				g_config.on_render();

			Renderer::after_render();
			System::present();
		}

		// update time
		Time::ticks++;
	}
}

void App::destroy()
{
	if (g_config.on_destroy)
		g_config.on_destroy();

	System::destroy();
	Renderer::destroy();
	Input::destroy();

#ifdef LEV_DEBUG
	Log::destroy();
#endif

	g_running = false;
}

void App::exit()
{
	g_running = false;
}

bool App::is_running()
{
	return g_running;
}

const AppConfig* App::config()
{
	return &g_config;
}

int App::window_width()
{
	return System::window_width();
}

int App::window_height()
{
	return System::window_height();
}

int App::draw_width()
{
	return System::draw_width();
}

int App::draw_height()
{
	return System::draw_height();
}
