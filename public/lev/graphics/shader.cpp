#include <lev/graphics/shader.h>
#include <lev/io/file_stream.h>
#include <backend/renderer.h>

using namespace lev;

Ref<ShaderBuffer> ShaderBuffer::create(u64 size)
{
	LEV_ASSERT(size > 0, "Size must be > 0");
	return Renderer::create_shader_buffer(size);
}

void ShaderBuffer::unbind()
{
	Renderer::unbind_shader_buffer();
}

//////////////////////////////////////////////////////////

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
		data.vertex_source = vertex;
		data.fragment_source = fragment;

		if (geometry)
			data.geometry_source = geometry;
	}
	else
	{
		char vtxsrc[LEV_SHADER_MAX_SIZE] = {0};
		char frgsrc[LEV_SHADER_MAX_SIZE] = {0};

		auto vtxfs = FileStream(vertex, "r");
		auto frgfs = FileStream(fragment, "r");

		LEV_ASSERT(vtxfs.size() <= LEV_SHADER_MAX_SIZE, "Vertex shader file size must not be above LEV_SHADER_MAX_SIZE");
		LEV_ASSERT(frgfs.size() <= LEV_SHADER_MAX_SIZE, "Fragment shader file size must not be above LEV_SHADER_MAX_SIZE");

		vtxfs.read(vtxsrc, vtxfs.size()).close();
		frgfs.read(frgsrc, frgfs.size()).close();

		data.vertex_source = vtxsrc;
		data.fragment_source = frgsrc;

		if (geometry)
		{
			char geosrc[LEV_SHADER_MAX_SIZE];
			auto geofs = FileStream(geometry, "r");

			LEV_ASSERT(geofs.size() <= LEV_SHADER_MAX_SIZE-1, "Geometry shader file size must not be above LEV_SHADER_MAX_SIZE");

			geofs.read(geosrc, geofs.size()).close();
			data.geometry_source = geosrc;
		}
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
		char cmpsrc[LEV_SHADER_MAX_SIZE] = {0};
		auto cmpfs = FileStream(compute, "r");
		
		LEV_ASSERT(cmpfs.size() <= LEV_SHADER_MAX_SIZE, "Compute shader file size must not be above LEV_SHADER_MAX_SIZE");
		
		cmpfs.read(cmpsrc, cmpfs.size()).close();
		data.compute_source = cmpsrc;
	}

	return Renderer::create_shader(data);
}

void Shader::unbind()
{
	Renderer::unbind_shader();
}
