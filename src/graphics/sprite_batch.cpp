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
	: m_transform_matrix(Mat3x2::identity())
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::initialize()
{
	const char* vertex =
		"#version 330 core\n"
		"layout (location = 0) in vec2 a_pos;\n"
		"layout (location = 1) in vec2 a_uv;\n"
		"layout (location = 2) in vec3 a_colour;\n"
		"out vec3 frag_mod_colour;\n"
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
		"in vec3 frag_mod_colour;\n"
		"uniform sampler2D u_texture_0;\n"
		"void main() {\n"
		"	frag_colour = texture(u_texture_0, frag_coord) * vec4(frag_mod_colour, 1.0);\n"
		"}";

	m_default_shader = Shader::create(vertex, fragment, true);
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
	if (!m_default_shader)
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

void SpriteBatch::quad()
{
	RenderBatch batch;

	batch.material = peek_material();
	batch.blend = peek_blend();

	batch.mesh = Renderer::create_mesh();
	{
		Vertex vertices[4];
		u32 indices[6];

		float width = batch.material.textures[0]->width();
		float height = batch.material.textures[0]->height();

		Quad quad = Quad(
			Vec2(0.0f,  0.0f),
			Vec2(0.0f,  height),
			Vec2(width, height),
			Vec2(width, 0.0f)
		);

		Quad uv = Quad(
			Vec2(0.0f, 1.0f),
			Vec2(0.0f, 0.0f),
			Vec2(1.0f, 0.0f),
			Vec2(1.0f, 1.0f)
		);

		GfxUtil::quad(vertices, indices, quad, uv, Colour::white());

		for (int i = 0; i < 4; i++)
			vertices[i].pos = Vec2::transform(vertices[i].pos, m_transform_matrix);

		batch.mesh->vertex_data(vertices, 4);
		batch.mesh->index_data(indices, 6);
	}

	m_batches.push_back(batch);
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
	return m_material_stack.pop_back();
}

Material& SpriteBatch::peek_material()
{
	return m_material_stack.back();
}

Ref<Shader> SpriteBatch::peek_shader()
{
	auto shd = peek_material().shader;

	if (shd)
		return shd;
	else
		return m_default_shader;
}

void SpriteBatch::push_blend(const BlendMode& blend)
{
	m_blend_stack.push_back(blend);
}

BlendMode SpriteBatch::pop_blend()
{
	return m_blend_stack.pop_back();
}

BlendMode& SpriteBatch::peek_blend()
{
	if (m_blend_stack.size() <= 0)
		return g_default_blend;

	return m_blend_stack.back();
}
