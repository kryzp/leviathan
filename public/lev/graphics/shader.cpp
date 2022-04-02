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

Ref<ShaderBuffer> ShaderBuffer::create(void* buf, u64 size)
{
	LEV_ASSERT(buf, "Buffer must not be nullptr");
	LEV_ASSERT(size >= 0, "Size must be >= 0");

	return Renderer::create_shader_buffer(buf, size);
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
		data.vertex_source = vertex;
		data.fragment_source = fragment;

		if (geometry)
			data.geometry_source = geometry;
	}
	else
	{
		char vtxsrc[LEV_SHADER_MAX_CHAR_SIZE];
		char frgsrc[LEV_SHADER_MAX_CHAR_SIZE];
		char geosrc[LEV_SHADER_MAX_CHAR_SIZE];

		FileStream(vertex, "r").read(vtxsrc, LEV_SHADER_MAX_CHAR_SIZE).close();
		FileStream(fragment, "r").read(frgsrc, LEV_SHADER_MAX_CHAR_SIZE).close();

		if (geometry)
			FileStream(geometry, "r").read(geosrc, LEV_SHADER_MAX_CHAR_SIZE).close();

		data.vertex_source = vtxsrc;
		data.fragment_source = frgsrc;
		data.geometry_source = geosrc;
	}

	return Renderer::create_shader(data);
}

Ref<Shader> Shader::create_compute(const char* compute, bool is_source)
{
	LEV_ASSERT(compute, "Compute shader must not be nullptr");

	ShaderData data = {0};
	data.type = SHADER_TYPE_COMPUTE;

	if (is_source)
		data.compute_source = compute;
	else
	{
		char cmpsrc[LEV_SHADER_MAX_CHAR_SIZE];
		FileStream(compute, "r").read(cmpsrc, LEV_SHADER_MAX_CHAR_SIZE);
		data.compute_source = cmpsrc;
	}

	return Renderer::create_shader(data);
}
