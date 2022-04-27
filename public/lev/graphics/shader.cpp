#include <lev/graphics/shader.h>
#include <lev/io/file_stream.h>
#include <backend/renderer.h>

using namespace lev;

Ref<ShaderBuffer> ShaderBuffer::create(u64 size)
{
	LEV_ASSERT(size > 0, "Size must be > 0");
	return Renderer::inst()->create_shader_buffer(size);
}

void ShaderBuffer::unbind()
{
	Renderer::inst()->unbind_shader_buffer();
}

//////////////////////////////////////////////////////////

Ref<Shader> Shader::create_single(const lev::String& file)
{
	LEV_ASSERT(file, "Data must not be nullptr")

	ShaderData data = {0};
	data.type = SHADER_TYPE_SINGLE;

	auto fs = FileStream(file.c_str(), "r");
	LEV_ASSERT(fs.size() <= LEV_SHADER_MAX_SIZE, "Compute shader file size must not be above LEV_SHADER_MAX_SIZE");
	fs.read(data.single_source, fs.size());

	return Renderer::inst()->create_shader(data);
}

Ref<Shader> Shader::create_seperated(const lev::String& vertex, const lev::String& fragment, const lev::String& geometry)
{
	LEV_ASSERT(vertex, "Vertex shader must not be nullptr");
	LEV_ASSERT(fragment, "Fragment shader must not be nullptr");

	ShaderData data = {0};
	data.type = SHADER_TYPE_SEPERATED | SHADER_TYPE_RENDER;

	auto vtxfs = FileStream(vertex.c_str(), "r");
	auto frgfs = FileStream(fragment.c_str(), "r");

	LEV_ASSERT(vtxfs.size() <= LEV_SHADER_MAX_SIZE, "Vertex shader file size must not be above LEV_SHADER_MAX_SIZE");
	LEV_ASSERT(frgfs.size() <= LEV_SHADER_MAX_SIZE, "Fragment shader file size must not be above LEV_SHADER_MAX_SIZE");

	vtxfs.read(data.seperated.vertex_source, vtxfs.size());
	frgfs.read(data.seperated.fragment_source, frgfs.size());

	if (!geometry.empty())
	{
		data.seperated.has_geometry = true;
		auto geofs = FileStream(geometry.c_str(), "r");
		LEV_ASSERT(geofs.size() <= LEV_SHADER_MAX_SIZE-1, "Geometry shader file size must not be above LEV_SHADER_MAX_SIZE");
		geofs.read(data.seperated.geometry_source, geofs.size());
	}

	return Renderer::inst()->create_shader(data);
}

Ref<Shader> Shader::create_compute_seperated(const lev::String& compute)
{
	LEV_ASSERT(compute, "Compute shader must not be nullptr");

	ShaderData data = {0};;
	data.type = SHADER_TYPE_SEPERATED | SHADER_TYPE_COMPUTE;

	auto cmpfs = FileStream(compute.c_str(), "r");
	LEV_ASSERT(cmpfs.size() <= LEV_SHADER_MAX_SIZE, "Compute shader file size must not be above LEV_SHADER_MAX_SIZE");
	cmpfs.read(data.seperated.compute_source, cmpfs.size());

	return Renderer::inst()->create_shader(data);
}

void Shader::unbind()
{
	Renderer::inst()->unbind_shader();
}
