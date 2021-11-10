#if LEVIATHAN_USE_OPENGL

#include <internal/graphics.h>
#include <internal/glad/include/glad/glad.h>

using namespace Lev;

namespace
{
	const char* default_vertex_src = "";
	const char* default_fragment_src = "";
	u32 default_shader;
}

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

u32 Graphics::create_shader(const char* vertex, const char* fragment)
{
	// todo
	return 0;
}

void Graphics::use_shader(u32 shader)
{
	glUseProgram(shader);
}

void Graphics::shader_set_u1i(u32 id, const char* name, int value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Graphics::shader_set_u1f(u32 id, const char* name, float value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

RendererType Graphics::renderer_type()
{
	return RendererType::OpenGL;
}

#endif
