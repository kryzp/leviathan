#include <lev/graphics/shader.h>
#include <backend/renderer.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace Lev;

Shader::Shader()
{
}

Shader::~Shader()
{
}

Ref<Shader> Shader::create(const char* vertex, const char* fragment)
{
	ShaderData data;
	{
		// todo: i dont like this should it be in the platform layer?
		
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

const UniformData& Shader::get_uniform_data(UniformFlags flags) const
{
	for (auto& uniform : data().uniforms)
	{
		if (uniform.flags == flags)
			return uniform;
	}

	return UniformData();
}
