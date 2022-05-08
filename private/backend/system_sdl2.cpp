#if LEV_USE_SDL2

#include <backend/system.h>
#include <backend/renderer.h>

#include <lev/core/app.h>
#include <lev/input/input.h>

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <third_party/glad/glad.h>
#include <SDL.h>

using namespace lev;
using namespace lev::bknd;

class SDL2System : public System
{
    SDL_Window* m_window;
	SDL_Joystick* m_joystick; // todo: add support for multiple joysticks

public:
	SDL2System()
		: m_window(nullptr)
		, m_joystick(nullptr)
	{
	}

	SystemProperties properties() override
	{
		SystemProperties p = {0};
		p.temp = 0;

		return p;
	}

    bool init(const Config& cfg) override
	{
#if _WIN32
        SetProcessDPIAware();
#endif

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
        {
            log::error("failed to initialize sdl2: %s", SDL_GetError());
            return false;
        }

        int flags = SDL_WINDOW_ALLOW_HIGHDPI;

        if (cfg.resizable)
            flags |= SDL_WINDOW_RESIZABLE;

#ifdef LEV_USE_OPENGL
        flags |= SDL_WINDOW_OPENGL;
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
        }
#endif

        m_window = SDL_CreateWindow(cfg.name, 0, 0, cfg.width, cfg.height, flags);

        if (!m_window)
        {
            log::error("failed to create window");
            return false;
        }

        SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		{
			m_joystick = SDL_JoystickOpen(0);

			if (!m_joystick)
				log::error("unable to open joystick");
		}

		show_cursor(cfg.cursor_visible);

        return true;
    }

    void destroy() override
    {
        if (m_window != nullptr)
            SDL_DestroyWindow(m_window);

        m_window = nullptr;

		if (m_joystick)
			SDL_JoystickClose(m_joystick);

        SDL_Quit();
    }

    void postinit() override
    {
#ifdef LEV_USE_OPENGL
        if (App::inst()->config().vsync)
            SDL_GL_SetSwapInterval(1);
        else
            SDL_GL_SetSwapInterval(0);
#endif
    }

    void update() override
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    App::inst()->exit();
                    break;

                case SDL_MOUSEWHEEL:
                    Input::inst()->on_mouse_wheel(e.wheel.x, e.wheel.y);
                    break;

                case SDL_MOUSEMOTION:
                    int spx, spy;
                    SDL_GetGlobalMouseState(&spx, &spy);
                    Input::inst()->on_mouse_screen_move(spx, spy);
                    Input::inst()->on_mouse_move(e.motion.x, e.motion.y);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    Input::inst()->on_mouse_down(e.button.button);
                    break;

                case SDL_MOUSEBUTTONUP:
                    Input::inst()->on_mouse_up(e.button.button);
                    break;

                case SDL_KEYDOWN:
                    Input::inst()->on_key_down(e.key.keysym.scancode);
                    break;

                case SDL_KEYUP:
                    Input::inst()->on_key_up(e.key.keysym.scancode);
                    break;

                case SDL_TEXTINPUT:
                    Input::inst()->on_text_utf8(e.text.text);
                    break;

				case SDL_JOYBUTTONDOWN:
					Input::inst()->on_joystick_button_down(e.jbutton.button);
					break;

				case SDL_JOYBUTTONUP:
					Input::inst()->on_joystick_button_up(e.jbutton.button);
					break;

				case SDL_JOYBALLMOTION:
					Input::inst()->on_joystick_ball(e.jball.ball);
					break;

				case SDL_JOYHATMOTION:
					Input::inst()->on_joystick_hat(e.jhat.value);
					break;

				case SDL_JOYAXISMOTION:
					float value;
					if (e.jaxis.value >= 0) value = static_cast<float>(e.jaxis.value) / 32767.0f;
					else value = static_cast<float>(e.jaxis.value) / 32768.0f;
					Input::inst()->on_joystick_motion((e.jaxis.axis == 0) ? JS_AXIS_H : JS_AXIS_V, value);
					break;

                default:
                    break;
            }
        }
    }

    void present() override
    {
#ifdef LEV_USE_OPENGL
        SDL_GL_SwapWindow(m_window);
#endif

        SDL_ShowWindow(m_window);
    }

    void set_window_position(int x, int y) override
    {
        SDL_SetWindowPosition(m_window, x, y);
    }

    void get_window_position(int* x, int* y) override
    {
        SDL_GetWindowPosition(m_window, x, y);
    }

    int window_width() override
    {
        int result = 0;
        SDL_GetWindowSize(m_window, &result, nullptr);
        return result;
    }

    int window_height() override
    {
        int result = 0;
        SDL_GetWindowSize(m_window, nullptr, &result);
        return result;
    }

    int draw_width() override
    {
        int result = 0;

#ifdef LEV_USE_OPENGL
        SDL_GL_GetDrawableSize(m_window, &result, nullptr);
#else
        result = window_width();
#endif

        return result;
    }

    int draw_height() override
    {
        int result = 0;

#ifdef LEV_USE_OPENGL
        SDL_GL_GetDrawableSize(m_window, nullptr, &result);
#else
        result = window_height();
#endif

        return result;
    }

    void sleep(u32 ms) override
    {
        if (ms > 0)
            SDL_Delay(ms);
    }

    u64 ticks() override
    {
        return SDL_GetTicks64();
    }

	void show_cursor(bool toggle) override
	{
		SDL_ShowCursor(toggle ? SDL_ENABLE : SDL_DISABLE);
	}

	bool cursor_visible() override
	{
		int result = SDL_ShowCursor(SDL_QUERY);

		if (result == SDL_ENABLE)
			return true;
		else if (result == SDL_DISABLE)
			return false;

		return true;
	}

    void* stream_from_file(const char* filepath, const char* mode) override
    {
        return SDL_RWFromFile(filepath, mode);
    }

    void* stream_from_memory(void* memory, s64 size) override
    {
        return SDL_RWFromMem(memory, size);
    }

    void* stream_from_const_memory(const void* memory, s64 size) override
    {
        return SDL_RWFromConstMem(memory, size);
    }

    s64 stream_read(void* stream, void* ptr, s64 size) override
    {
        return SDL_RWread((SDL_RWops*)stream, ptr, sizeof(char), size);
    }

    s64 stream_write(void* stream, const void* ptr, s64 size) override
    {
        return SDL_RWwrite((SDL_RWops*)stream, ptr, sizeof(char), size);
    }

    s64 stream_seek(void* stream, s64 offset) override
    {
        return SDL_RWseek((SDL_RWops*)stream, offset, RW_SEEK_SET);
    }

    s64 stream_size(void* stream) override
    {
        return SDL_RWsize((SDL_RWops*)stream);
    }

    s64 stream_position(void* stream) override
    {
        return SDL_RWtell((SDL_RWops*)stream);
    }

    void stream_close(void* stream) override
    {
        SDL_RWclose((SDL_RWops*)stream);
    }

    void* gl_context_create() override
    {
#ifdef LEV_USE_OPENGL
        return SDL_GL_CreateContext(m_window);
#endif

        return nullptr;
    }

    void gl_context_make_current(void* context) override
    {
#ifdef LEV_USE_OPENGL
        SDL_GL_MakeCurrent(m_window, context);
#endif
    }

    void gl_context_destroy(void* context) override
    {
#ifdef LEV_USE_OPENGL
        SDL_GL_DeleteContext(context);
#endif
    }

    bool gl_load_glad_loader() override
    {
        return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    }
};

System* System::inst()
{
    static SDL2System* instance = nullptr;
    if (!instance) { instance = new SDL2System(); }
    return instance;
}

#endif
