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

Ref<Shader> Shader::create(const char* vertex, const char* fragment, const char* geometry, bool is_source)
{
	LEV_ASSERT(vertex, "Vertex shader must not be nullptr");
	LEV_ASSERT(fragment, "Fragment shader must not be nullptr");

	ShaderData data = {0};

	data.type =
		SHADER_TYPE_FRAGMENT |
		SHADER_TYPE_VERTEX;

	if (geometry)
		data.type |= SHADER_TYPE_GEOMETRY;

	if (is_source)
	{
		MemUtil::copy(data.vertex_source, vertex, LEV_SHADER_CHAR_SIZE);
		MemUtil::copy(data.fragment_source, fragment, LEV_SHADER_CHAR_SIZE);
		if (geometry) MemUtil::copy(data.geometry_source, geometry, LEV_SHADER_CHAR_SIZE);
	}
	else
	{
		FileStream(vertex, "r").read(data.vertex_source, LEV_SHADER_CHAR_SIZE).close();
		FileStream(fragment, "r").read(data.fragment_source, LEV_SHADER_CHAR_SIZE).close();
		if (geometry) FileStream(geometry, "r").read(data.geometry_source, LEV_SHADER_CHAR_SIZE).close();
	}

	Ref<Shader> result = Renderer::create_shader(data);
	return result;
}

Ref<Shader> Shader::create_compute(const char* compute, bool is_source)
{
	LEV_ASSERT(compute, "Compute shader must not be nullptr");

	ShaderData data = {0};

	data.type = SHADER_TYPE_COMPUTE;

	if (is_source)
		MemUtil::copy(data.compute_source, compute, LEV_SHADER_CHAR_SIZE);
	else
		FileStream(compute, "r").read(data.compute_source, LEV_SHADER_CHAR_SIZE);

	Ref<Shader> result = Renderer::create_shader(data);
	return result;
}
