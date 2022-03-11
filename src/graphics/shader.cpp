#include <lev/graphics/shader.h>
#include <lev/io/file_stream.h>
#include <backend/renderer.h>

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
		MemUtil::copy(data.vertex_source, vertex, LEV_SHADER_CHAR_SIZE);
		MemUtil::copy(data.fragment_source, fragment, LEV_SHADER_CHAR_SIZE);
	}
	else
	{
		io::FileStream(vertex, "r").read(data.vertex_source, LEV_SHADER_CHAR_SIZE);
		io::FileStream(fragment, "r").read(data.fragment_source, LEV_SHADER_CHAR_SIZE);
	}

	Ref<Shader> result = Renderer::create_shader(data);
	return result;
}
