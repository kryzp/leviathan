#include <lev/core/app.h>
#include <lev/core/util.h>

#include <lev/math/colour.h>
#include <lev/math/timer.h>

#include <lev/input/input.h>

#include <backend/system.h>
#include <backend/renderer.h>

#include <iostream>

using namespace lev;

void App::start(const Config& cfg)
{
	m_config = cfg;

	if (!init())
	{
		log::error("exiting...");
		return;
	}

	App::run();
	App::destroy();
}

bool App::init()
{
#ifdef LEV_DEBUG
	if (!log::init())
	{
		// r/hmmm
		log::error("failed to initialize logging");
		return false;
	}
#endif

	if (!Input::inst()->init())
	{
		log::error("failed to initialize input");
		return false;
	}

	if (!System::inst()->init(m_config))
	{
		log::error("failed to initialize system");
		return false;
	}

	if (!Renderer::inst()->init())
	{
		log::error("failed to initialize rendering");
		return false;
	}

	if (m_config.on_init)
		m_config.on_init();

	m_running = true;

	System::inst()->postinit();

	return true;
}

void App::run()
{
	u64 lag = 0;
	u64 prev_ticks = 0;

	const u64 TIME_TGT = static_cast<u64>(1000.0f / (float)m_config.target_fps);

	while (m_running)
	{
		System::inst()->update();

		// time + update
		{
			Time::loops++;

			u64 ticks = System::inst()->ticks();
			u64 diff = ticks - prev_ticks;
			prev_ticks = ticks;
			lag += diff;
			
            while (lag < TIME_TGT)
            {
                System::inst()->sleep(TIME_TGT - lag);
                ticks = System::inst()->ticks();
                diff = ticks - prev_ticks;
                prev_ticks = ticks;
                lag += diff;
            }
			
			while (lag >= TIME_TGT)
			{
				lag -= TIME_TGT;
				Time::delta = 1.0f / m_config.target_fps;
				
				Time::prev_milliseconds = Time::milliseconds;
				Time::milliseconds += TIME_TGT;

				Time::prev_elapsed = Time::elapsed;
				Time::elapsed += Time::delta;

				if (m_config.on_update)
					m_config.on_update();
				
				Input::inst()->update();
			}
		}

		// render
		{
			Renderer::inst()->before_render();

			if (m_config.on_render)
				m_config.on_render();

			Renderer::inst()->after_render();
			System::inst()->present();
		}
	}
}

void App::destroy()
{
	if (m_config.on_destroy)
		m_config.on_destroy();

	System::inst()->destroy();
	Renderer::inst()->destroy();

#ifdef LEV_DEBUG
	log::destroy();
#endif

	m_running = false;
}

void App::exit()
{
	m_running = false;

	if (m_config.on_exit)
		m_config.on_exit();
}

bool App::is_running()
{
	return m_running;
}

const Config& App::config()
{
	return m_config;
}

int App::window_width()
{
	return System::inst()->window_width();
}

int App::window_height()
{
	return System::inst()->window_height();
}

Size2 App::window_size()
{
	return Size2(
		System::inst()->window_width(),
		System::inst()->window_height()
	);
}

int App::draw_width()
{
	return System::inst()->draw_width();
}

int App::draw_height()
{
	return System::inst()->draw_height();
}

Size2 App::draw_size()
{
	return Size2(
		System::inst()->draw_width(),
		System::inst()->draw_height()
	);
}

float App::fps()
{
	return m_config.target_fps;
}

void App::clear(const Colour& colour)
{
	Renderer::inst()->clear(colour);
}
