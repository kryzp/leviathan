#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/gfxutil.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/font.h>
#include <backend/renderer.h>

using namespace lev;

namespace
{
	BlendMode g_default_blend = {
		.equation_rgb   = BlendEquation::ADD,
		.equation_alpha = BlendEquation::ADD,
		.func_src_rgb   = BlendFunc::SRC_ALPHA,
		.func_dst_rgb   = BlendFunc::ONE_MINUS_SRC_ALPHA,
		.func_src_alpha = BlendFunc::ONE,
		.func_dst_alpha = BlendFunc::ONE
	};
}

SpriteBatch::SpriteBatch()
	: m_initialized(false)
	, m_transform_matrix(Mat3x2::identity())
{
	m_blend_stack.push_back(g_default_blend);

	Material mat;
	mat.textures[0] = nullptr;
	mat.samplers[0] = TextureSampler::pixel();
	m_material_stack.push_back(mat);
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::initialize()
{
	// generic shader
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

		m_material_stack[0].shader = Shader::create(vertex, fragment, true);
	}

	// font shader
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
			"	vec4 colour = texture(u_texture_0, frag_coord);\n"
			"	frag_colour = vec4(colour.r, colour.r, colour.r, colour.r) * frag_mod_colour;\n"
			"}";

		m_font_shader = Shader::create(vertex, fragment, true);
#endif
	}

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
			b.material.shader = m_material_stack[0].shader;

		b.material.shader->use();
		b.material.shader->set(Shader::PROJECTION, proj);
		b.material.shader->set(Shader::RESOLUTION, Vec2F(
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

	Vertex transformed[vtxcount];
	MemUtil::copy(transformed, vtx, sizeof(Vertex) * vtxcount);

	for (int i = 0; i < vtxcount; i++)
		transformed[i].pos = Vec2F::transform(transformed[i].pos, m_transform_matrix);

	batch.mesh->vertex_data(transformed, vtxcount);
	batch.mesh->index_data(idx, idxcount);

	m_batches.push_back(batch);
}

void SpriteBatch::push_quad(const Quad& quad, const Colour& colour)
{
	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(
		vertices, indices,
		quad,
		Quad(
			Vec2F(0.0f, 0.0f),
			Vec2F(0.0f, 1.0f),
			Vec2F(1.0f, 1.0f),
			Vec2F(1.0f, 0.0f)
		),
		colour
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_triangle(const Triangle& tri, const Colour& colour)
{
	Vertex vertices[3];
	u32 indices[3];

	GfxUtil::tri(
		vertices, indices,
		tri,
		Triangle(
			Vec2F(0.0f, 0.0f),
			Vec2F(0.5f, 1.0f),
			Vec2F(1.0f, 0.0f)
		),
		colour
	);

	push_vertices(vertices, 3, indices, 6);
}

void SpriteBatch::push_texture(const Ref<Texture>& tex, const Colour& colour)
{
	set_texture(tex);

	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(
		vertices, indices,
		Quad(RectF(
			0.0f,
			0.0f,
			tex->width(),
			tex->height()
		)),
		Quad(
			Vec2F(0.0f, 0.0f),
			Vec2F(0.0f, 1.0f),
			Vec2F(1.0f, 1.0f),
			Vec2F(1.0f, 0.0f)
		),
		colour
	);

	push_vertices(vertices, 4, indices, 6);
}


void SpriteBatch::push_texture(const TextureRegion& tex, const Colour& colour)
{
	set_texture(tex.source);

	Vertex vertices[4];
	u32 indices[6];

	// ( f l o a t )

	GfxUtil::quad(
		vertices, indices,
		Quad(RectF(
			0.0f,
			0.0f,
			tex.bounds.w,
			tex.bounds.h
		)),
		Quad(
			Vec2F(
				(float)tex.bounds.left() / (float)tex.source->width(),
				(float)tex.bounds.top() / (float)tex.source->height()
			),
			Vec2F(
				(float)tex.bounds.left() / (float)tex.source->width(),
				(float)tex.bounds.bottom() / (float)tex.source->height()
			),
			Vec2F(
				(float)tex.bounds.right() / (float)tex.source->width(),
				(float)tex.bounds.bottom() / (float)tex.source->height()
			),
			Vec2F(
				(float)tex.bounds.right() / (float)tex.source->width(),
				(float)tex.bounds.top() / (float)tex.source->height()
			)
		),
		colour
	);

	push_vertices(vertices, 4, indices, 6);
}

// this clearly is super basic
// no newlines, or anything
// but ive spent so long on the git now that i'll do this later

void SpriteBatch::push_string(const char* str, const Ref<Font>& font, const Colour& colour)//int extra_spacing=0?
{
	push_string(str, font, [&](FontCharacter c, int idx) { return Vec2F::zero(); }, colour);
}

void SpriteBatch::push_string(const char* str, const Ref<Font>& font, const std::function<Vec2F(FontCharacter,int)>& offsetfn, const Colour& colour)
{
	push_shader(m_font_shader); // engineer gamig 4

	const auto& atlas = font->atlas();
	const auto& info = font->info();

	int cursorx = 0;
	for (int i = 0; i < StrUtil::length(str); i++)
	{
		auto c = font->character(str[i]);

		push_matrix(Mat3x2::create_translation(
			Vec2F(cursorx + c.draw_offset.x, c.draw_offset.y) +
			offsetfn(c, i)
		));

		push_texture(atlas.region(c.bbox), colour);

		pop_matrix();

		cursorx +=
			c.advance_x +
			font->kern_advance(str[i], str[i+1]);
	}

	pop_shader();
}

void SpriteBatch::set_texture(const Ref<Texture>& tex, int idx)
{
	m_material_stack.back().textures[idx] = tex;
}

void SpriteBatch::set_sampler(const TextureSampler& sampler, int idx)
{
	m_material_stack.back().samplers[idx] = sampler;
}

void SpriteBatch::reset_texture(int idx)
{
	m_material_stack.back().textures[idx].reset();
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

void SpriteBatch::push_shader(const Ref<Shader>& shader)
{
	auto mat = m_material_stack.back();
	mat.shader = shader;
	push_material(mat);
}

void SpriteBatch::pop_shader()
{
	pop_material();
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
