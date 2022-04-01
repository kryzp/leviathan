#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/gfxutil.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/font.h>
#include <backend/renderer.h>

using namespace lev;

#define FRAGMODE_ALL (Colour(1.0f, 0.0f, 0.0f, 0.0f))
#define FRAGMODE_ALPHA (Colour(0.0f, 1.0f, 0.0f, 0.0f))
#define FRAGMODE_RED (Colour(0.0f, 0.0f, 1.0f, 0.0f))
#define FRAGMODE_SILHOUETTE (Colour(0.0f, 0.0f, 0.0f, 1.0f))

namespace
{
	BlendMode g_default_blend = {
		.equation_rgb   = BLEND_EQUATION_ADD,
		.equation_alpha = BLEND_EQUATION_ADD,
		.func_src_rgb   = BLEND_FUNC_SRC_ALPHA,
		.func_dst_rgb   = BLEND_FUNC_ONE_MINUS_SRC_ALPHA,
		.func_src_alpha = BLEND_FUNC_ONE,
		.func_dst_alpha = BLEND_FUNC_ONE
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
			"layout (location = 3) in vec4 a_mode;\n"
			"out vec2 frag_coord;\n"
			"out vec4 frag_mod_colour;\n"
			"out vec4 frag_mode;\n"
			"uniform mat4 u_projection;\n"
			"void main() {\n"
			"	gl_Position = u_projection * vec4(a_pos, 0.0, 1.0);\n"
			"	frag_mod_colour = a_colour;\n"
			"	frag_coord = a_uv;\n"
			"	frag_mode = a_mode;\n"
			"}";

		const char* fragment =
			"#version 330 core\n"
			"out vec4 frag_colour;\n"
			"in vec2 frag_coord;\n"
			"in vec4 frag_mod_colour;\n"
			"in vec4 frag_mode;\n"
			"uniform sampler2D u_texture_0;\n"
			"void main() {\n"
			"	vec4 texcol = texture(u_texture_0, frag_coord);\n"
			"	frag_colour = frag_mode.r * texcol   * frag_mod_colour + "
			"                 frag_mode.g * texcol.a * frag_mod_colour + "
			"                 frag_mode.b * texcol.r * frag_mod_colour + "
			"                 frag_mode.a * frag_mod_colour;\n"
			"}";
#endif

		m_material_stack[0].shader = Shader::create(vertex, fragment, nullptr, true);
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

void SpriteBatch::push_quad(const Quad& quad, const Colour& colour, const Colour& mode)
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
		colour,
		mode
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_triangle(const Triangle& tri, const Colour& colour, const Colour& mode)
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
		colour,
		mode
	);

	push_vertices(vertices, 3, indices, 3);
}

void SpriteBatch::push_texture(const TextureRegion& tex, const Colour& colour, const Colour& mode)
{
	set_texture(tex.source);

	Vertex vertices[4];
	u32 indices[6];

	Float2 texsize = tex.source->size();

	GfxUtil::quad(
		vertices, indices,
		Quad(RectF(
			0.0f,
			0.0f,
			tex.bounds.w,
			tex.bounds.h
		)),
		Quad(
			Vec2F(tex.bounds.left(), tex.bounds.top()) / texsize,
			Vec2F(tex.bounds.left(), tex.bounds.bottom()) / texsize,
			Vec2F(tex.bounds.right(), tex.bounds.bottom()) / texsize,
			Vec2F(tex.bounds.right(), tex.bounds.top()) / texsize
		),
		colour,
		mode
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_texture(const Ref<Texture>& tex, const Colour& colour, const Colour& mode)
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
		colour,
		mode
	);

	push_vertices(vertices, 4, indices, 6);
}

// todo make the font rendering not suck lol

void SpriteBatch::push_text(const char* str, const Font& font, TextAlign align, const Colour& colour)
{
	push_text(str, font, [&](Font::Character c, int idx) -> lev::Vec2F { return Vec2F::zero(); }, align, colour);
}

void SpriteBatch::push_text(const char* str, const Font& font, const std::function<Vec2F(Font::Character,int)>& offsetfn, TextAlign align, const Colour& colour)
{
	const auto& atlas = font.atlas();

	int cursorx = 0;
	for (int i = 0; i < StrUtil::length(str); i++)
	{
		auto c = font.character(str[i]);

		push_matrix(Mat3x2::create_translation(
			Vec2F(cursorx + c.draw_offset.x, c.draw_offset.y) +
			offsetfn(c, i)
		));

		push_texture(atlas.region(c.bbox), colour, FRAGMODE_RED);

		pop_matrix();

		cursorx +=
			c.advance_x +
			font.kern_advance(str[i], str[i+1]);
	}
}

void SpriteBatch::push_circle(const Circle& circle, u32 accuracy, const Colour& colour)
{
	float dtheta = Calc::TAU / accuracy;

	for (float theta = 0.0f; theta < Calc::TAU; theta += dtheta)
	{
		Triangle triangle;
		triangle.a = circle.position;
		triangle.b = Vec2F::from_angle(theta         , circle.radius) + circle.position;
		triangle.c = Vec2F::from_angle(theta + dtheta, circle.radius) + circle.position;

		push_triangle(triangle, colour, FRAGMODE_SILHOUETTE);
	}
}

void SpriteBatch::push_line(const Line& line, float thickness, const Colour& colour)
{
	Vec2F dir  = line.direction();
	Vec2F perp = dir.perpendicular() * thickness;

	Quad quad(
		line.a + perp + (Vec2F(-thickness,  thickness)*dir),
		line.a - perp + (Vec2F(-thickness, -thickness)*dir),
		line.b - perp + (Vec2F( thickness, -thickness)*dir),
		line.b + perp + (Vec2F( thickness,  thickness)*dir)
	);

	push_quad(quad, colour, FRAGMODE_SILHOUETTE);
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
