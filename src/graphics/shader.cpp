#include <lev/graphics/shader.h>
#include <backend/renderer.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace lev;
using namespace lev::gfx;

Shader::Shader()
{
}

Shader::~Shader()
{
}

Ref<Shader> Shader::create(const char* vertex, const char* fragment, bool is_source)
{
	ShaderData data;

	if (is_source)
	{
		data.vertex_source = vertex;
		data.fragment_source = fragment;
	}
	else
	{
		// todo: once filestream is finished use that instead of the std::ifstream

		std::stringstream buffer;

		std::ifstream vertex_file(vertex);
		std::ifstream fragment_file(fragment);
		
		buffer << vertex_file.rdbuf();
		data.vertex_source = buffer.str().c_str();

		buffer.str(std::string());

		buffer << fragment_file.rdbuf();
		data.fragment_source = buffer.str().c_str();
	}

	Ref<Shader> result = Renderer::create_shader(data);
	return result;
}

const char* Shader::uniform_name(UniformFlags flags) const
{
	for (auto& uniform : uniforms())
	{
		if (uniform.flags == flags)
			return uniform.name;
	}

	return nullptr;
}
