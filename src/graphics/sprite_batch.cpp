#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/gfxutil.h>
#include <lev/containers/string.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

namespace
{
	BlendMode g_default_blend = {
		.equation_rgb = gfx::BlendEquation::ADD,
		.equation_alpha = gfx::BlendEquation::ADD,
		.func_src_rgb = gfx::BlendFunc::SRC_ALPHA,
		.func_dst_rgb = gfx::BlendFunc::ONE_MINUS_SRC_ALPHA,
		.func_src_alpha = gfx::BlendFunc::ONE,
		.func_dst_alpha = gfx::BlendFunc::ONE
	};
}

SpriteBatch::SpriteBatch()
	: m_initialized(false)
	, m_transform_matrix(Mat3x2::identity())
{
	m_blend_stack.push_back(g_default_blend);

	gfx::Material mat;
	mat.textures = { nullptr };
	mat.samplers = { gfx::TextureSampler::pixel() };
	m_material_stack.push_back(mat);

}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::initialize()
{
#ifdef LEV_USE_OPENGL
	const char* vertex =
		"#version 330 core\n"
		"layout (location = 0) in vec2 a_pos;\n"
		"layout (location = 1) in vec2 a_uv;\n"
		"layout (location = 2) in vec4 a_colour;\n"
		"out vec4 frag_mod_colour;\n"
		"out vec2 frag_coord;\n"
		"uniform mat4 u_projection;\n"
		"void main() {\n"
		"	gl_Position = u_projection * vec4(a_pos, 0.0, 1.0);\n"
		"	frag_mod_colour = a_colour;\n"
		"	frag_coord = a_uv;\n"
		"}";

	const char* fragment =
		"#version 330 core\n"
		"out vec4 frag_colour;\n"
		"in vec2 frag_coord;\n"
		"in vec4 frag_mod_colour;\n"
		"uniform sampler2D u_texture_0;\n"
		"void main() {\n"
		"	frag_colour = texture(u_texture_0, frag_coord) * frag_mod_colour;\n"
		"}";
#endif

	m_default_shader = Shader::create(vertex, fragment, true);
	m_material_stack.front().shader = m_default_shader;

	m_initialized = true;
}

void SpriteBatch::render(const Ref<Framebuffer>& framebuffer)
{
	render(Mat4x4::create_orthographic(
		framebuffer ? framebuffer->width() : App::draw_width(),
		framebuffer ? framebuffer->height() : App::draw_height(),
		0.0f, 10000.0f
	), framebuffer);
}

void SpriteBatch::render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer)
{
	if (!m_initialized)
		initialize();

	RenderPass pass;
	pass.target = framebuffer;

	for (auto& b : m_batches)
	{
		if (!b.material.shader)
			b.material.shader = m_default_shader;

		b.material.shader->use();
		b.material.shader->set(Shader::PROJECTION, proj);
		b.material.shader->set(Shader::RESOLUTION, Vec2(
			framebuffer ? framebuffer->width() : App::draw_width(),
			framebuffer ? framebuffer->height() : App::draw_height()
		));

		render_batch(pass, b);
	}

	m_batches.clear();
}

void SpriteBatch::render_batch(RenderPass& pass, const RenderBatch& b)
{
	pass.blend = b.blend;
	pass.material = b.material;
	pass.mesh = b.mesh;
	Renderer::render(pass);
}

void SpriteBatch::push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount)
{
	RenderBatch batch;
	batch.material = peek_material();
	batch.blend = peek_blend();

	batch.mesh = Renderer::create_mesh();

	// engineer gamingeing (pt2 i think?)
	Vertex* transformed = (Vertex*)MemUtil::alloc(sizeof(Vertex) * vtxcount);
	MemUtil::copy(transformed, vtx, sizeof(Vertex) * vtxcount);

	for (int i = 0; i < vtxcount; i++)
		transformed[i].pos = Vec2::transform(transformed[i].pos, m_transform_matrix);

	batch.mesh->vertex_data(transformed, vtxcount);
	batch.mesh->index_data(idx, idxcount);

	MemUtil::free(transformed);

	m_batches.push_back(batch);
}

void SpriteBatch::push_quad(const Colour& colour)
{
	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(
		vertices, indices,
		0.0f, 0.0f,
		peek_texture()->width(), peek_texture()->height(),
		colour
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_quad(const Quad& quad, const Colour& colour)
{
	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(vertices, indices, quad, colour);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_triangle(const Triangle& tri, const Colour& colour)
{
	Vertex vertices[3];
	u32 indices[3];

	GfxUtil::tri(vertices, indices, tri, colour);

	push_vertices(vertices, 3, indices, 6);
}

void SpriteBatch::push_texture(const Ref<Texture>& tex, const TextureSampler& sampler, const Colour& colour)
{
	set_texture(tex, sampler, 0);

	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(
		vertices, indices,
		0.0f, 0.0f,
		tex->width(), tex->height(),
		colour
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::set_texture(const Ref<Texture>& tex, const TextureSampler& sampler, int idx)
{
	m_material_stack.back().textures[idx] = tex;
	m_material_stack.back().samplers[idx] = sampler;
}

Ref<Texture> SpriteBatch::peek_texture(int idx)
{
	return peek_material().textures[idx];
}

const TextureSampler& SpriteBatch::peek_sampler(int idx)
{
	return peek_material().samplers[idx];
}

void SpriteBatch::set_shader(const Ref<Shader>& shader)
{
	m_material_stack.back().shader = shader;
}

void SpriteBatch::reset_shader()
{
	set_shader(nullptr);
}

Ref<Shader> SpriteBatch::peek_shader()
{
	return peek_material().shader;
}

void SpriteBatch::push_matrix(const Mat3x2& matrix)
{
	m_matrix_stack.push_back(m_transform_matrix);
	m_transform_matrix = matrix * m_transform_matrix;
}

Mat3x2 SpriteBatch::pop_matrix()
{
	auto val = m_transform_matrix;
	m_transform_matrix = m_matrix_stack.pop_back();
	return val;
}

const Mat3x2& SpriteBatch::peek_matrix() const
{
	return m_transform_matrix;
}

void SpriteBatch::push_material(const Material& material)
{
	m_material_stack.push_back(material);
}

Material SpriteBatch::pop_material()
{
	if (m_material_stack.size() > 1)
		return m_material_stack.pop_back();

	return m_material_stack.back();
}

const Material& SpriteBatch::peek_material()
{
	return m_material_stack.back();
}

void SpriteBatch::push_blend(const BlendMode& blend)
{
	m_blend_stack.push_back(blend);
}

BlendMode SpriteBatch::pop_blend()
{
	if (m_blend_stack.size() > 1)
		return m_blend_stack.pop_back();

	return m_blend_stack.back();
}

const BlendMode& SpriteBatch::peek_blend()
{
	return m_blend_stack.back();
}
