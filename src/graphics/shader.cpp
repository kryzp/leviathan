#include <lev/graphics/shader.h>
#include <internal/graphics.h>

using namespace Lev;

Shader::Shader()
{
}

Shader::Shader(const char* vertex, const char* fragment)
{
	id = Graphics::create_shader(vertex, fragment);
}

void Shader::use()
{
	Graphics::use_shader(id);
}

void Shader::set(const char* name, bool value) const
{
	Graphics::shader_set_u1i(id, name, (int)value);
}

void Shader::set(const char* name, int value) const
{
	Graphics::shader_set_u1i(id, name, value);
}

void Shader::set(const char* name, float value) const
{
	Graphics::shader_set_u1f(id, name, value);
}
