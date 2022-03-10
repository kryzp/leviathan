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
		data.vertex_source = vertex;
		data.fragment_source = fragment;
	}
	else
	{
		char vtxfile[512] = {0};
		char fmtfile[512] = {0};

		io::FileStream(vertex, "r").read_all(vtxfile);
		io::FileStream(fragment, "r").read_all(fmtfile);

		data.vertex_source = vtxfile;
		data.fragment_source = fmtfile;
	}

	Ref<Shader> result = Renderer::create_shader(data);
	return result;
}
