#include <lev/graphics/shader.h>
#include <lev/io/file_stream.h>
#include <backend/renderer.h>

using namespace lv;

Ref<ShaderBuffer> ShaderBuffer::create(u64 size)
{
	LEV_ASSERT(size > 0, "Size must be > 0");
	return bknd::Renderer::inst()->create_shader_buffer(size);
}

void ShaderBuffer::unbind()
{
	bknd::Renderer::inst()->unbind_shader_buffer();
}

//////////////////////////////////////////////////////////

Ref<Shader> Shader::create_single(const String& path)
{
	LEV_ASSERT(path.empty(), "Path must not be empty")

	ShaderData data = {0};
	data.type = SHADER_TYPE_SINGLE;

	auto fs = FileStream(path.c_str(), "r");
	LEV_ASSERT(fs.size() <= LEV_SHADER_MAX_SIZE, "Compute shader file size must not be above LEV_SHADER_MAX_SIZE");
	fs.read(data.single_source, fs.size());

	return bknd::Renderer::inst()->create_shader(data);
}

Ref<Shader> Shader::create_seperated(const String& vertex, const String& fragment, const String& geometry)
{
	LEV_ASSERT(!vertex.empty(), "Vertex path must not be empty");
	LEV_ASSERT(!fragment.empty(), "Fragment path must not be empty");

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

	return bknd::Renderer::inst()->create_shader(data);
}

Ref<Shader> Shader::create_compute_seperated(const String& path)
{
	LEV_ASSERT(!path.empty(), "Path must not be empty");

	ShaderData data = {0};;
	data.type = SHADER_TYPE_SEPERATED | SHADER_TYPE_COMPUTE;

	auto cmpfs = FileStream(path.c_str(), "r");
	LEV_ASSERT(cmpfs.size() <= LEV_SHADER_MAX_SIZE, "Compute shader file size must not be above LEV_SHADER_MAX_SIZE");
	cmpfs.read(data.seperated.compute_source, cmpfs.size());

	return bknd::Renderer::inst()->create_shader(data);
}

void Shader::unbind()
{
	bknd::Renderer::inst()->unbind_shader();
}
