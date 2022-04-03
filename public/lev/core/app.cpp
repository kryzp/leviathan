#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/math/colour.h>
#include <lev/math/timer.h>
#include <lev/input/input.h>

#include <backend/system.h>
#include <backend/renderer.h>

#include <iostream>

using namespace lev;

namespace
{
	bool g_running = false;
	Config g_config;
}

void App::start(const Config& cfg)
{
	g_config = cfg;

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
		Log::error("failed to initialize system");
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

	System::postinit();

	return true;
}

void App::run()
{
	u64 lag = 0;
	u64 prev_ticks = 0;

	const float TIME_TGT = 1.0f / (float)g_config.target_tps;

	while (g_running)
	{
		System::update();

		// time + update
		{
			Time::loops++;

			u64 ticks = System::ticks();
			u64 diff = ticks - prev_ticks;
			prev_ticks = ticks;
			lag += diff;
			
            while (lag < TIME_TGT)
            {
                System::sleep(TIME_TGT - lag);
                ticks = System::ticks();
                diff = ticks - prev_ticks;
                prev_ticks = ticks;
                lag += diff;
            }

			while (lag >= TIME_TGT)
			{
				lag -= TIME_TGT;
				Time::delta = TIME_TGT;
				
				Time::prev_milliseconds = Time::milliseconds;
				Time::milliseconds += TIME_TGT;

				Time::prev_elapsed = Time::elapsed;
				Time::elapsed += Time::delta;

				Input::update();

				if (g_config.on_update)
					g_config.on_update();
			}
		}

		// render
		{
			Renderer::before_render();

			if (g_config.on_render)
				g_config.on_render();

			Renderer::after_render();
			System::present();
		}
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

const Config& App::config()
{
	return g_config;
}

int App::window_width()
{
	return System::window_width();
}

int App::window_height()
{
	return System::window_height();
}

Size2 App::window_size()
{
	return Size2(
		System::window_width(),
		System::window_height()
	);
}

int App::draw_width()
{
	return System::draw_width();
}

int App::draw_height()
{
	return System::draw_height();
}

Size2 App::draw_size()
{
	return Size2(
		System::draw_width(),
		System::draw_height()
	);
}

float App::fps()
{
	return 1.0f / Time::delta;
}

void App::clear(const Colour& colour)
{
	Renderer::clear(colour);
}
