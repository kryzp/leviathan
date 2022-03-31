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

namespace
{
	SDL_Window* g_window;
}

bool System::init(const Config* cfg)
{
#if _WIN32
	SetProcessDPIAware();
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
	{
		Log::error("failed to initialize sdl2");
		return false;
	}

	int flags = SDL_WINDOW_ALLOW_HIGHDPI;

	if (cfg->resizable)
		flags |= SDL_WINDOW_RESIZABLE;

#ifdef LEV_USE_OPENGL
	flags |= SDL_WINDOW_OPENGL;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
#endif

	g_window = SDL_CreateWindow(cfg->name, 0, 0, cfg->width, cfg->height, flags);
	
	if (!g_window)
	{
		Log::error("failed to create window");
		return false;
	}

	SDL_SetWindowPosition(g_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	return true;
}

void System::destroy()
{
	if (g_window != nullptr)
		SDL_DestroyWindow(g_window);

	g_window = nullptr;

	SDL_Quit();
}

void System::postinit()
{
#ifdef LEV_USE_OPENGL
	if (App::config().vsync)
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
				App::exit();
				break;

			case SDL_MOUSEWHEEL:
				Input::on_mouse_wheel(e.wheel.x, e.wheel.y);
				break;

			case SDL_MOUSEMOTION:
				int spx, spy;
				SDL_GetGlobalMouseState(&spx, &spy);
				Input::on_mouse_screen_move(spx, spy);
				Input::on_mouse_move(e.motion.x, e.motion.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
				Input::on_mouse_down(e.button.button);
				break;

			case SDL_MOUSEBUTTONUP:
				Input::on_mouse_up(e.button.button);
				break;

			case SDL_KEYDOWN:
				Input::on_key_down(e.key.keysym.scancode);
				break;

			case SDL_KEYUP:
				Input::on_key_up(e.key.keysym.scancode);
				break;

			case SDL_TEXTINPUT:
                Input::on_text_utf8(e.text.text);
				break;

			default:
				break;
		}
	}
}

void System::present()
{
#ifdef LEV_USE_OPENGL
	SDL_GL_SwapWindow(g_window);
#endif

	SDL_ShowWindow(g_window);
}

void System::set_window_position(int x, int y)
{
	SDL_SetWindowPosition(g_window, x, y);
}

void System::get_window_position(int* x, int* y)
{
	SDL_GetWindowPosition(g_window, x, y);
}

int System::window_width()
{
	int result = 0;
	SDL_GetWindowSize(g_window, &result, nullptr);
	return result;
}

int System::window_height()
{
	int result = 0;
	SDL_GetWindowSize(g_window, nullptr, &result);
	return result;
}

int System::draw_width()
{
	int result = 0;

#ifdef LEV_USE_OPENGL
	SDL_GL_GetDrawableSize(g_window, &result, nullptr);
#else
	result = window_width();
#endif

	return result;
}

int System::draw_height()
{
	int result = 0;

#ifdef LEV_USE_OPENGL
	SDL_GL_GetDrawableSize(g_window, nullptr, &result);
#else
	result = window_height();
#endif

	return result;
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
	return SDL_GL_CreateContext(g_window);
#endif

	return nullptr;
}

void System::gl_context_make_current(void* context)
{
#ifdef LEV_USE_OPENGL
	SDL_GL_MakeCurrent(g_window, context);
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
