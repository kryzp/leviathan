#include <lev/core/app.h>
#include <lev/core/util.h>
#include <lev/math/colour.h>
#include <lev/input/input.h>

#include <backend/system.h>
#include <backend/renderer.h>
#include <backend/audio.h>

using namespace lev;

void App::start(const Config& cfg)
{
	m_config = cfg;

	if (!init())
	{
		log::error("exiting...");
		return;
	}

	run();
	destroy();
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

	if (!bknd::System::inst()->init(m_config))
	{
		log::error("failed to initialize system");
		return false;
	}

	if (!bknd::Renderer::inst()->init())
	{
		log::error("failed to initialize rendering");
		return false;
	}

	if (!bknd::Audio::inst()->init())
	{
		log::error("failed to initialize audio");
		return false;
	}

    if (!Input::inst()->init())
    {
        log::error("failed to initialize input");
        return false;
    }

	if (m_config.on_init)
		m_config.on_init();

	m_running = true;

	bknd::System::inst()->postinit();

	return true;
}

void App::run()
{
	u64 lag = 0;
	u64 prev_ticks = 0;

	const u64 TIME_TGT = static_cast<u64>(1000.0f / (float)m_config.target_fps);

	time::delta = 1.0f / m_config.target_fps;

	while (m_running)
	{
		bknd::System::inst()->update();

		// time + update
		{
			time::run_loops++;

			u64 ticks = bknd::System::inst()->ticks();
			u64 diff = ticks - prev_ticks;
			prev_ticks = ticks;
			lag += diff;
			
            while (lag < TIME_TGT)
            {
				bknd::System::inst()->sleep(TIME_TGT - lag);
                ticks = bknd::System::inst()->ticks();
                diff = ticks - prev_ticks;
                prev_ticks = ticks;
                lag += diff;
            }

			uint64_t time_maximum = m_config.max_updates * TIME_TGT;
			if (lag > time_maximum)
				lag = time_maximum;

			while (lag >= TIME_TGT)
			{
				lag -= TIME_TGT;

				time::prev_milliseconds = time::milliseconds;
				time::milliseconds += TIME_TGT;

				time::prev_elapsed = time::elapsed;
				time::elapsed += time::delta;

				time::step_count++;

				if (m_config.on_update)
					m_config.on_update();

				Input::inst()->update();
				bknd::Audio::inst()->update();
			}
		}

		// render
		{
			bknd::Renderer::inst()->before_render();

			if (m_config.on_render)
				m_config.on_render();

			bknd::Renderer::inst()->after_render();
			bknd::System::inst()->present();
		}
	}
}

void App::destroy()
{
	if (m_config.on_destroy)
		m_config.on_destroy();

	log::print("destroying input manager");
	Input::inst()->destroy();
	delete Input::inst();

	log::print("destroying audio backend");
	bknd::Audio::inst()->destroy();
	delete bknd::Audio::inst();

	log::print("destroying renderer backend");
	bknd::Renderer::inst()->destroy();
	delete bknd::Renderer::inst();

	log::print("destroying system backend");
	bknd::System::inst()->destroy();
	delete bknd::System::inst();

#ifdef LEV_DEBUG
	log::print("destroying logger");
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

bool App::is_running() const
{
	return m_running;
}

const Config& App::config()
{
	return m_config;
}

void App::show_cursor(bool toggle)
{
	bknd::System::inst()->show_cursor(toggle);
}

bool App::cursor_visible()
{
	return bknd::System::inst()->cursor_visible();
}

Vec2I App::window_position()
{
	Vec2I pos;
	bknd::System::inst()->get_window_position(&pos.x, &pos.y);
	return pos;
}

void App::window_position(int x, int y)
{
	bknd::System::inst()->set_window_position(x, y);
}

int App::window_width()
{
	return bknd::System::inst()->window_width();
}

int App::window_height()
{
	return bknd::System::inst()->window_height();
}

Size2 App::window_size()
{
	return Size2(
		bknd::System::inst()->window_width(),
		bknd::System::inst()->window_height()
	);
}

int App::draw_width()
{
	return bknd::System::inst()->draw_width();
}

int App::draw_height()
{
	return bknd::System::inst()->draw_height();
}

Size2 App::draw_size()
{
	return Size2(
		bknd::System::inst()->draw_width(),
		bknd::System::inst()->draw_height()
	);
}

float App::fps() const
{
	return m_config.target_fps;
}

void App::clear(const Colour& colour)
{
	bknd::Renderer::inst()->clear(colour);
}
