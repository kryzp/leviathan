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

#define M_WINDOW ((SDL_Window*)m_window)

bool System::init(const Config& cfg)
{
#if _WIN32
	SetProcessDPIAware();
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
	{
		log::error("failed to initialize sdl2");
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

	SDL_SetWindowPosition(M_WINDOW, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	return true;
}

void System::destroy()
{
	if (M_WINDOW != nullptr)
		SDL_DestroyWindow(M_WINDOW);

	m_window = nullptr;

	SDL_Quit();
}

void System::postinit()
{
#ifdef LEV_USE_OPENGL
	if (App::inst()->config().vsync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
#endif
}

void System::update()
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

			default:
				break;
		}
	}
}

void System::present()
{
#ifdef LEV_USE_OPENGL
	SDL_GL_SwapWindow(M_WINDOW);
#endif

	SDL_ShowWindow(M_WINDOW);
}

void System::set_window_position(int x, int y)
{
	SDL_SetWindowPosition(M_WINDOW, x, y);
}

void System::get_window_position(int* x, int* y)
{
	SDL_GetWindowPosition(M_WINDOW, x, y);
}

int System::window_width()
{
	int result = 0;
	SDL_GetWindowSize(M_WINDOW, &result, nullptr);
	return result;
}

int System::window_height()
{
	int result = 0;
	SDL_GetWindowSize(M_WINDOW, nullptr, &result);
	return result;
}

int System::draw_width()
{
	int result = 0;

#ifdef LEV_USE_OPENGL
	SDL_GL_GetDrawableSize(M_WINDOW, &result, nullptr);
#else
	result = window_width();
#endif

	return result;
}

int System::draw_height()
{
	int result = 0;

#ifdef LEV_USE_OPENGL
	SDL_GL_GetDrawableSize(M_WINDOW, nullptr, &result);
#else
	result = window_height();
#endif

	return result;
}

void System::sleep(u32 ms)
{
	if (ms > 0)
		SDL_Delay(ms);
}

u64 System::ticks()
{
	return SDL_GetTicks64();
}

void* System::stream_from_file(const char* filepath, const char* mode)
{
    return SDL_RWFromFile(filepath, mode);
}

void* System::stream_from_memory(void* memory, s64 size)
{
	return SDL_RWFromMem(memory, size);
}

void* System::stream_from_const_memory(const void* memory, s64 size)
{
	return SDL_RWFromConstMem(memory, size);
}

s64 System::stream_read(void* stream, void* ptr, s64 size)
{
    return SDL_RWread((SDL_RWops*)stream, ptr, sizeof(char), size);
}

s64 System::stream_write(void* stream, const void* ptr, s64 size)
{
    return SDL_RWwrite((SDL_RWops*)stream, ptr, sizeof(char), size);
}

s64 System::stream_seek(void* stream, s64 offset)
{
    return SDL_RWseek((SDL_RWops*)stream, offset, RW_SEEK_SET);
}

s64 System::stream_size(void* stream)
{
    return SDL_RWsize((SDL_RWops*)stream);
}

s64 System::stream_position(void* stream)
{
    return SDL_RWtell((SDL_RWops*)stream);
}

void System::stream_close(void* stream)
{
	SDL_RWclose((SDL_RWops*)stream);
}

void* System::gl_context_create()
{
#ifdef LEV_USE_OPENGL
	return SDL_GL_CreateContext(M_WINDOW);
#endif

	return nullptr;
}

void System::gl_context_make_current(void* context)
{
#ifdef LEV_USE_OPENGL
	SDL_GL_MakeCurrent(M_WINDOW, context);
#endif
}

void System::gl_context_destroy(void* context)
{
#ifdef LEV_USE_OPENGL
	SDL_GL_DeleteContext(context);
#endif
}

bool System::gl_load_glad_loader()
{
	return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}

#endif
