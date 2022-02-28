#if LEV_USE_SDL2

#include <backend/platform.h>
#include <backend/renderer.h>
#include <lev/core/app.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// am i mixing this too much with the opengl backend... :/ messy!
#include <third_party/glad/glad.h>
#include <SDL.h>

using namespace Lev;

namespace
{
	SDL_Window* g_window;
}

bool Platform::init(const AppConfig* cfg)
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

	if (App::renderer_type() == RendererType::OpenGL)
	{
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
	}

	g_window = SDL_CreateWindow(cfg->name, 0, 0, cfg->width, cfg->height, flags);
	
	if (!g_window)
	{
		std::cout << "failed to create window" << std::endl;
		return false;
	}

	SDL_SetWindowPosition(g_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	return true;
}

void Platform::destroy()
{
	if (g_window != nullptr)
		SDL_DestroyWindow(g_window);

	g_window = nullptr;

	SDL_Quit();
}

void Platform::prepare()
{
	if (App::renderer_type() == RendererType::OpenGL)
		SDL_GL_SetSwapInterval(1);
}

void Platform::update()
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
	if (Renderer::renderer_type() == RendererType::OpenGL)
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

	if (Renderer::renderer_type() == RendererType::OpenGL)
		SDL_GL_GetDrawableSize(g_window, &result, nullptr);
	else
		result = window_width();

	return result;
}

int Platform::draw_height()
{
	int result = 0;

	if (Renderer::renderer_type() == RendererType::OpenGL)
		SDL_GL_GetDrawableSize(g_window, nullptr, &result);
	else
		result = window_height();

	return result;
}

void* Platform::gl_context_create()
{
	if (App::renderer_type() == RendererType::OpenGL)
		return SDL_GL_CreateContext(g_window);

	return nullptr;
}

void Platform::gl_context_make_current(void* context)
{
	if (App::renderer_type() == RendererType::OpenGL)
		SDL_GL_MakeCurrent(g_window, context);
}

void Platform::gl_context_destroy(void* context)
{
	if (App::renderer_type() == RendererType::OpenGL)
		SDL_GL_DeleteContext(context);
}

bool Platform::gl_load_glad_loader()
{
	return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}

#endif
