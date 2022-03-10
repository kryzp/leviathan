#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/gfxutil.h>
#include <lev/containers/string.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

namespace
{
}

SpriteBatch::SpriteBatch()
	: m_initialized(false)
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::initialize()
{
	m_default_blend = {
		.func_rgb = gfx::BlendFunction::ADD,
		.func_alpha = gfx::BlendFunction::ADD,
		.factor_src_rgb = gfx::BlendFactor::SRC_ALPHA,
		.factor_dst_rgb = gfx::BlendFactor::ONE_MINUS_SRC_ALPHA,
		.factor_src_alpha = gfx::BlendFactor::ONE,
		.factor_dst_alpha = gfx::BlendFactor::ZERO
	};

	m_default_shader = Shader::create(
		"#version 330 core\nlayout (location = 0) in vec2 a_pos;\nlayout (location = 1) in vec2 a_uv;\nlayout (location = 2) in vec3 a_colour;\nout vec3 in_colour;\nout vec2 in_uv;\nuniform mat4 u_projection;\nvoid main() {\ngl_Position = u_projection * vec4(a_pos, 0.0, 1.0);\nin_colour = a_colour;\nin_uv = a_uv;\n}",
		"#version 330 core\nout vec4 frag_colour;\nin vec3 in_colour;\nin vec2 in_uv;\nuniform sampler2D u_texture;\nvoid main() {\nfrag_colour = texture(u_texture, in_uv) * vec4(in_colour, 1.0);\n}",
		true
	);

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
		b.material->shader->set(Shader::PROJECTION, proj);
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

void SpriteBatch::texture(const Ref<Texture>& tex)
{
	if (!m_initialized)
		initialize();

	RenderBatch batch;
	{
		batch.mesh = Renderer::create_mesh();
		{
			Vertex vertices[4];
			u32 indices[6];

			float width = tex->width();
			float height = tex->height();

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

			GfxUtil::quad(vertices, indices, quad, uv, Colour::WHITE);

			for (int i = 0; i < 4; i++)
				vertices[i].pos = Vec2::transform(vertices[i].pos, m_transform_matrix);

			batch.mesh->vertex_data(vertices, 4);
			batch.mesh->index_data(indices, 6);
		}

		batch.material = create_ref<Material>();
		{
			batch.material->shader = peek_shader();
			batch.material->texture = tex;
			batch.material->sampler = {
				.filter = TextureFilter::NEAREST,
				.wrap_x = TextureWrap::CLAMP,
				.wrap_y = TextureWrap::CLAMP
			};
		}

		batch.blend = peek_blend();
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

void SpriteBatch::push_blend(const BlendMode& blend)
{
	m_blend_stack.push_back(blend);
}

BlendMode SpriteBatch::pop_blend()
{
	return m_blend_stack.pop_back();
}

const BlendMode& SpriteBatch::peek_blend() const
{
	if (m_blend_stack.size() <= 0)
		return m_default_blend;

	return m_blend_stack.back();
}

void SpriteBatch::push_shader(const Ref<Shader>& shader)
{
	m_shader_stack.push_back(shader);
}

Ref<Shader> SpriteBatch::pop_shader()
{
	return m_shader_stack.pop_back();
}

Ref<Shader> SpriteBatch::peek_shader()
{
	if (m_shader_stack.size() <= 0)
		return m_default_shader;

	return m_shader_stack.back();
}
