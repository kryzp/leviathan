#include <lev/graphics/shader.h>
#include <lev/io/file_stream.h>
#include <backend/renderer.h>

using namespace lev;

Shader::Shader()
{
}

Shader::~Shader()
{
}

Ref<Shader> Shader::create(const char* vertex, const char* fragment, bool is_source)
{
	LEV_ASSERT(vertex, "Vertex shader must not be nullptr");
	LEV_ASSERT(fragment, "Fragment shader must not be nullptr");

	ShaderData data;

	if (is_source)
	{
		MemUtil::copy(data.vertex_source, vertex, LEV_SHADER_CHAR_SIZE);
		MemUtil::copy(data.fragment_source, fragment, LEV_SHADER_CHAR_SIZE);
	}
	else
	{
		FileStream(vertex, "r").read(data.vertex_source, LEV_SHADER_CHAR_SIZE).close();
		FileStream(fragment, "r").read(data.fragment_source, LEV_SHADER_CHAR_SIZE).close();
	}

	Ref<Shader> result = Renderer::create_shader(data);
	return result;
}
