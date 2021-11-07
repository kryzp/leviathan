#if LEVIATHAN_USE_SDL2

#include "platform.h"
#include "graphics.h"

#include <lev/core/app.h>

#include <SDL.h>

using namespace LEV;

namespace
{
	SDL_Window* g_window;
}

bool Platform::init(const Config* cfg)
{
	int flags = SDL_WINDOW_ALLOW_HIGHDPI;

	if (cfg->resizable)
		flags |= SDL_WINDOW_RESIZABLE;

	if (App::renderer_type() == RendererType::OpenGL)
		flags |= SDL_WINDOW_OPENGL;

	g_window = SDL_CreateWindow(cfg->name, 0, 0, cfg->width, cfg->height, flags);
	SDL_SetWindowPosition(g_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	return true;
}

void Platform::shutdown()
{
}

void Platform::prepare()
{
}

void Platform::frame()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			App::exit();
	}
}

void Platform::present()
{
	if (Graphics::renderer_type() == RendererType::OpenGL)
		SDL_GL_SwapWindow(g_window);

	SDL_ShowWindow(g_window);
}

int Platform::window_width()
{
	int result = 0;
	SDL_GetWindowSize(g_window, &result, nullptr);
	return result;
}

int Platform::window_height()
{
	int result = 0;
	SDL_GetWindowSize(g_window, nullptr, &result);
	return result;
}

int Platform::draw_width()
{
	int result = 0;

	if (Graphics::renderer_type() == RendererType::OpenGL)
		SDL_GL_GetDrawableSize(g_window, &result, nullptr);
	else
		result = window_width();

	return result;
}

int Platform::draw_height()
{
	int result = 0;

	if (Graphics::renderer_type() == RendererType::OpenGL)
		SDL_GL_GetDrawableSize(g_window, nullptr, &result);
	else
		result = window_height();

	return result;
}

#endif
