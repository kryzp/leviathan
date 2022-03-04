#if LEV_USE_SDL2

#include <backend/system.h>
#include <backend/renderer.h>
#include <lev/core/app.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <third_party/glad/glad.h>
#include <SDL.h>

using namespace Lev;

namespace
{
	SDL_Window* g_window;
}

bool System::init(const AppConfig* cfg)
{
#if _WIN32
	SetProcessDPIAware();
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)
	{
		std::cout << "failed to initialize sdl2" << std::endl;
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
		std::cout << "failed to create window" << std::endl;
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

void System::prepare()
{
#ifdef LEV_USE_OPENGL
	SDL_GL_SetSwapInterval(1);
#endif
}

void System::update()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			App::exit();
	}
}

void System::present()
{
#ifdef LEV_USE_OPENGL
	SDL_GL_SwapWindow(g_window);
#endif

	SDL_ShowWindow(g_window);
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
