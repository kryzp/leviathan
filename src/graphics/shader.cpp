#include <lev/graphics/shader.h>
#include <backend/graphics.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace Lev;

Ref<Shader> Shader::create(const char* vertex, const char* fragment)
{
	ShaderData data;
	{
		// todo: i dont like this should it be in the platform layer?
		
		std::stringstream buffer;

		std::ifstream vertex_file(vertex);
		std::ifstream fragment_file(fragment);
		
		buffer << vertex_file.rdbuf();
		strncpy(data.vertex_source, buffer.str().c_str(), 512);

		buffer.str(std::string());

		buffer << fragment_file.rdbuf();
		strncpy(data.fragment_source, buffer.str().c_str(), 512);
	}

	Ref<Shader> result = Graphics::create_shader(data);
	return result;
}

u32 Shader::id() const
{
	return m_id;
}
