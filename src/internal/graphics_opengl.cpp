#if LEVIATHAN_USE_OPENGL

#include "graphics.h"

using namespace Lev;

bool Graphics::init()
{
	return true;
}

void Graphics::shutdown()
{
}

void Graphics::before_render()
{
}

void Graphics::after_render()
{
}

RendererType Graphics::renderer_type()
{
	return RendererType::OpenGL;
}

#endif
