#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/gfxutil.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/font.h>
#include <lev/graphics/shader.h>

#include <backend/renderer.h>

using namespace lev;

SpriteBatch::SpriteBatch()
	: m_initialized(false)
	, m_transform_matrix(Mat3x2::identity())
	, m_material_stack()
	, m_layer_stack()
	, m_depth_stack()
	, m_stencil_stack()
	, m_scissor_stack()
	, m_viewport_stack()
{
	m_layer_stack.push_back(0.0f);
	m_blend_stack.push_back(BlendMode::generic());
	m_depth_stack.push_back(COMPARE_ALWAYS);
	m_stencil_stack.push_back(Compare::none());
	m_scissor_stack.push_back(RectI::zero());
	m_viewport_stack.push_back(RectI::zero());
	m_material_stack.push_back(Material());
}

SpriteBatch::~SpriteBatch()
{
	if (m_material_stack[0].shader())
		delete m_material_stack[0].shader();
	m_material_stack[0].set_shader(nullptr);
}

void SpriteBatch::initialize()
{
	// generic shader
	{
#ifdef LEV_USE_OPENGL
		char vertex[512] =
			"#version 330 core\n"
			"layout (location = 0) in vec2 a_pos;\n"
			"layout (location = 1) in vec2 a_uv;\n"
			"layout (location = 2) in vec4 a_colour;\n"
			"layout (location = 3) in vec4 a_mode;\n"
			"out vec2 frag_coord;\n"
			"out vec4 frag_mod_colour;\n"
			"out vec4 frag_mode;\n"
			"uniform mat4 lev_projection;\n"
			"void main() {\n"
			"	gl_Position = lev_projection * vec4(a_pos, 0.0, 1.0);\n"
			"	frag_mod_colour = a_colour;\n"
			"	frag_coord = a_uv;\n"
			"	frag_mode = a_mode;\n"
			"}";

		char fragment[512] =
			"#version 330 core\n"
			"out vec4 frag_colour;\n"
			"in vec2 frag_coord;\n"
			"in vec4 frag_mod_colour;\n"
			"in vec4 frag_mode;\n"
			"uniform sampler2D lev_texture_0;\n"
			"void main() {\n"
			"	vec4 texcol = texture(lev_texture_0, frag_coord);\n"
			"	frag_colour = frag_mode.r * texcol   * frag_mod_colour + "
			"                 frag_mode.g * texcol.a * frag_mod_colour + "
			"                 frag_mode.b * texcol.r * frag_mod_colour + "
			"                 frag_mode.a * frag_mod_colour;\n"
			"}";

		ShaderData data = {0};
		data.type = SHADER_TYPE_SEPERATED | SHADER_TYPE_RENDER;

		cstr::copy(data.seperated.vertex_source, vertex, 512);
		cstr::copy(data.seperated.fragment_source, fragment, 512);

		m_material_stack[0].set_shader(Renderer::inst()->create_shader(data));
		m_material_stack[0].set_texture(0, nullptr);
		m_material_stack[0].set_sampler(0, TextureSampler::pixel());
#endif
	}

	m_initialized = true;
}

void SpriteBatch::render(const Framebuffer* framebuffer, int sort_mode)
{
	render(Mat4x4::create_orthographic(
		framebuffer ? framebuffer->width() : App::inst()->draw_width(),
		framebuffer ? framebuffer->height() : App::inst()->draw_height(),
		0.0f, 10000.0f
	), framebuffer, sort_mode);
}

void SpriteBatch::render(const Mat4x4& proj, const Framebuffer* framebuffer, int sort_mode)
{
	if (!m_initialized)
		initialize();

	RenderPass pass;
	pass.instance_count = 0; // for now
	pass.target = framebuffer;

	// todo: sort by layers
	// [sort_mode]

	for (auto& b : m_batches)
	{
		if (!b.material.shader())
			b.material.set_shader(m_material_stack[0].shader());

		b.material.shader()->use()
			.set(Shader::PROJECTION, proj)
			.set(Shader::RESOLUTION, Vec2F(
				framebuffer ? framebuffer->width() : App::inst()->draw_width(),
				framebuffer ? framebuffer->height() : App::inst()->draw_height()
			));

		render_batch(pass, b);

		delete b.mesh;
	}

	m_batches.clear();
}

void SpriteBatch::render_batch(RenderPass& pass, const RenderBatch& b)
{
	pass.blend = b.blend;
	pass.material = b.material;
	pass.mesh = b.mesh;
	pass.depth = b.depth;
	pass.stencil = b.stencil;
	pass.viewport = b.viewport;
	pass.scissor = b.scissor;

	Renderer::inst()->render(pass);
}

void SpriteBatch::push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount)
{
	RenderBatch batch;
	batch.layer = peek_layer();
	batch.material = peek_material();
	batch.blend = peek_blend();
	batch.depth = peek_depth();
	batch.stencil = peek_stencil();
	batch.viewport = peek_viewport();
	batch.scissor = peek_scissor();

	batch.mesh = Renderer::inst()->create_mesh();
	{
        Vertex transformed[vtxcount]; // note: will only work on some compilers. MSVC wont allow this so compiling with msvc would mean using 'new' or just Vector<Vertex>
        mem::copy(transformed, vtx, sizeof(Vertex) * vtxcount); // i have no clue why this is necessary but it doesnt work without it lol sssskssksskssksksksksksk

        for (int i = 0; i < vtxcount; i++)
            transformed[i].pos = Vec2F::transform(vtx[i].pos, m_transform_matrix);

        batch.mesh->vertex_data(transformed, vtxcount);
        batch.mesh->index_data(idx, idxcount);
	}

	m_batches.push_back(batch);
}

void SpriteBatch::push_quad(const Quad& quad, const Colour& colour, u8 mode)
{
	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(
		vertices, indices,
		quad,
		Quad(RectF::one()),
		colour,
		mode
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_triangle(const Triangle& tri, const Colour& colour, u8 mode)
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

void SpriteBatch::push_texture(const TextureRegion& tex, const Colour& colour, u8 mode)
{
	set_texture(tex.source);

	Vertex vertices[4];
	u32 indices[6];

	Float2 texsize = tex.source->size();

	GfxUtil::quad(
		vertices, indices,
		Quad(RectF(0.0f, 0.0f, tex.bounds.w, tex.bounds.h)),
		Quad(tex.bounds) / texsize,
		colour,
		mode
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_texture(Texture* tex, const Colour& colour, u8 mode)
{
	set_texture(tex);

	Vertex vertices[4];
	u32 indices[6];

	GfxUtil::quad(
		vertices, indices,
		Quad(RectF(0.0f, 0.0f, tex->width(), tex->height())),
		Quad(RectF::one()),
		colour,
		mode
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_string(
	const char* str,
	const Font* font,
	u8 align,
	const Colour& colour,
	int monospaced
)
{
	push_string(str, font, [&](Font::Character c, int idx) -> lev::Vec2F { return Vec2F::zero(); }, align, colour, monospaced);
}

void SpriteBatch::push_string(
	const char* str,
	const Font* font,
	const std::function<Vec2F(Font::Character,int)>& offset_fn,
	u8 align,
	const Colour& colour,
	int monospaced
)
{
	// todo: new line support

	const auto& atlas = font->atlas();

	switch (align)
	{
		case TEXT_ALIGN_LEFT:
			push_matrix(Mat3x2::identity());
			break;
		
		case TEXT_ALIGN_CENTRE:
			push_matrix(Mat3x2::create_translation(Vec2F(-font->string_width(str) / 2.0f, 0.0f)));
			break;

		case TEXT_ALIGN_RIGHT:
			push_matrix(Mat3x2::create_translation(Vec2F(-font->string_width(str), 0.0f)));
			break;
	}

	float cursor_x = 0.0f;

	for (int i = 0; i < cstr::length(str); i++)
	{
		auto c = font->character(str[i]);

		push_matrix(Mat3x2::create_translation(
			Vec2F(cursor_x + c.draw_offset.x, c.draw_offset.y) +
			offset_fn(c, i)
		));

		push_texture(atlas.region(c.bbox), colour, SB_RENDER_MODE_RED);

		pop_matrix();

		if (monospaced)
		{
			cursor_x += monospaced;
		}
		else
		{
			cursor_x +=
				c.advance_x +
				font->kern_advance(str[i], str[i + 1]);
		}
	}

	pop_matrix();
}

void SpriteBatch::push_circle(const Circle& circle, u32 accuracy, const Colour& colour)
{
	float dtheta = calc::TAU / accuracy;

	for (float theta = 0.0f; theta < calc::TAU; theta += dtheta)
	{
		Triangle triangle;
		triangle.a = circle.position;
		triangle.b = Vec2F::from_angle(theta         , circle.radius) + circle.position;
		triangle.c = Vec2F::from_angle(theta + dtheta, circle.radius) + circle.position;

		push_triangle(triangle, colour, SB_RENDER_MODE_SILHOUETTE);
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

	push_quad(quad, colour, SB_RENDER_MODE_SILHOUETTE);
}

void SpriteBatch::set_texture(Texture* tex, int idx)
{
	peek_material().set_texture(idx, tex);
}

void SpriteBatch::set_sampler(const TextureSampler& sampler, int idx)
{
	peek_material().set_sampler(idx, sampler);
}

void SpriteBatch::reset_texture(int idx)
{
	peek_material().set_texture(idx, nullptr);
}

Texture* SpriteBatch::peek_texture(int idx)
{
	return peek_material().texture(idx);
}

const TextureSampler& SpriteBatch::peek_sampler(int idx)
{
	return peek_material().sampler(idx);
}

void SpriteBatch::push_shader(Shader* shd)
{
	push_material(peek_material());
	peek_material().set_shader(shd);
}

Shader* SpriteBatch::pop_shader()
{
	return pop_material().shader();
}

Shader* SpriteBatch::peek_shader()
{
	return peek_material().shader();
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

Material& SpriteBatch::peek_material()
{
	return m_material_stack.back();
}

void SpriteBatch::push_stencil(Compare stencil)
{
	m_stencil_stack.push_back(stencil);
}

Compare SpriteBatch::pop_stencil()
{
	return m_stencil_stack.pop_back();
}

Compare& SpriteBatch::peek_stencil()
{
	return m_stencil_stack.back();
}

void SpriteBatch::push_depth(u8 depth)
{
	m_depth_stack.push_back(depth);
}

u8 SpriteBatch::pop_depth()
{
	return m_depth_stack.pop_back();
}

u8& SpriteBatch::peek_depth()
{
	return m_depth_stack.back();
}

void SpriteBatch::push_scissor(const RectI& scissor)
{
	m_scissor_stack.push_back(scissor);
}

RectI SpriteBatch::pop_scissor()
{
	return m_scissor_stack.pop_back();
}

RectI& SpriteBatch::peek_scissor()
{
	return m_scissor_stack.back();
}

void SpriteBatch::push_viewport(const RectI& viewport)
{
	m_viewport_stack.push_back(viewport);
}

RectI SpriteBatch::pop_viewport()
{
	return m_viewport_stack.pop_back();
}

RectI& SpriteBatch::peek_viewport()
{
	if (m_viewport_stack.size() > 0)
		return m_viewport_stack.back();

	return m_viewport_stack.back();
}

void SpriteBatch::push_layer(float layer)
{
	m_layer_stack.push_back(layer);
}

float SpriteBatch::pop_layer()
{
	if (m_layer_stack.size() > 1)
		return m_layer_stack.pop_back();

	return m_layer_stack.back();
}

float& SpriteBatch::peek_layer()
{
	return m_layer_stack.back();
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

Mat3x2& SpriteBatch::peek_matrix()
{
	return m_transform_matrix;
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

BlendMode& SpriteBatch::peek_blend()
{
	return m_blend_stack.back();
}
