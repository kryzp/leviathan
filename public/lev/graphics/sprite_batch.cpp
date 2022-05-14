#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/gfxutil.h>
#include <lev/graphics/texture.h>
#include <lev/graphics/font.h>
#include <lev/graphics/shader.h>
#include <lev/core/app.h>

#include <algorithm>

#include <backend/renderer.h>

using namespace lv;

SpriteBatch::SpriteBatch()
	: m_initialized(false)
	, m_default_material(nullptr)
	, m_curr_matrix(Mat3x2::identity())
	, m_material_stack()
	, m_layer_stack()
	, m_depth_stack()
	, m_stencil_stack()
	, m_scissor_stack()
	, m_viewport_stack()
{
}

void SpriteBatch::initialize()
{
	// generic shader also horrible code please look away: avert your eyes, child.
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
			"uniform sampler2D u_texture;\n"
			"void main() {\n"
			"	vec4 texcol = texture(u_texture, frag_coord);\n"
			"	frag_colour = frag_mode.r * texcol   * frag_mod_colour + "
			"                 frag_mode.g * texcol.a * frag_mod_colour + "
			"                 frag_mode.b * texcol.r * frag_mod_colour + "
			"                 frag_mode.a * frag_mod_colour;\n"
			"}";

		ShaderData data = {0};
		data.type_flags = SHADER_TYPE_RENDER;

		cstr::copy(data.vertex_source, vertex, 512);
		cstr::copy(data.fragment_source, fragment, 512);

		m_default_material = Material::create(bknd::Renderer::inst()->create_shader(data));
#endif
	}

	m_mesh = bknd::Renderer::inst()->create_mesh();

	{
		m_curr_batch.material = peek_material();
		m_curr_batch.depth = peek_depth();
		m_curr_batch.stencil = peek_stencil();
		m_curr_batch.blend = peek_blend();
		m_curr_batch.layer = peek_layer();
		m_curr_batch.viewport = peek_viewport();
		m_curr_batch.scissor = peek_scissor();
	}

	m_initialized = true;
}

void SpriteBatch::render(const Ref<RenderTarget>& target, SpriteSort sort_mode)
{
	render(Mat4x4::create_orthographic_ext(
		0.0f,
		target ? target->width() : App::inst()->draw_width(),
		target ? target->height() : App::inst()->draw_height(),
		0.0f,
		0.01f,
		10000.0f
	), target, sort_mode);
}

void SpriteBatch::render(const Mat4x4& proj, const Ref<RenderTarget>& target, SpriteSort sort_mode)
{
	if (!m_initialized)
		initialize();

	RenderPass pass;
	pass.instance_count = 0; // for now
	pass.target = target;

	switch (sort_mode)
	{
		// front to back
		case SPRITE_SORT_FTB:
			std::sort(m_batches.begin(), m_batches.end(), [](const RenderBatch& a, const RenderBatch& b) -> bool { return a.layer < b.layer; });
			break;

		// back to front
		case SPRITE_SORT_BTF:
			std::sort(m_batches.begin(), m_batches.end(), [](const RenderBatch& a, const RenderBatch& b) -> bool { return a.layer > b.layer; });
			break;

		// dont do anything - order of render calls
		case SPRITE_SORT_DEFERRED:
			break;
	}

	for (auto& b : m_batches)
		render_batch(pass, b, proj);

	clear();
}

void SpriteBatch::clear()
{
	m_curr_matrix = Mat3x2::identity();

	m_batches.clear();
	m_material_stack.clear();
	m_layer_stack.clear();
	m_material_stack.clear();
	m_matrix_stack.clear();
	m_blend_stack.clear();
	m_depth_stack.clear();
	m_stencil_stack.clear();
	m_scissor_stack.clear();
	m_viewport_stack.clear();
	m_colour_mode_stack.clear();
}

void SpriteBatch::render_batch(RenderPass& pass, const RenderBatch& b, const Mat4x4& proj)
{
	const int res_x = pass.target ? pass.target->width() : App::draw_width();
	const int res_y = pass.target ? pass.target->height() : App::draw_height();

	pass.blend = b.blend;
	pass.material = b.material;
	pass.depth = b.depth;
	pass.stencil = b.stencil;
	pass.viewport = b.viewport;
	pass.scissor = b.scissor;

	pass.material = b.material;
	{
		if (!pass.material)
			pass.material = m_default_material;

		pass.material->set_shader(b.shader);
		pass.material->set_texture(b.texture, 0);
		pass.material->set_sampler(b.sampler, 0);

		pass.material->shader()->use()
			.set(Shader::PROJECTION, proj)
			.set(Shader::RESOLUTION, Vec2I(res_x, res_y));
	}

	if (pass.viewport == RectI::zero())
		pass.viewport = RectI(0, 0, res_x, res_y);

	m_mesh->vertex_data(b.vertices.begin(), b.vertices.size());
	m_mesh->index_data(b.indices.begin(), b.indices.size());
	pass.mesh = m_mesh;

	pass.perform();
}

void SpriteBatch::push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount, bool flip_vertically)
{
	RenderBatch batch = m_curr_batch;

	// mesh
	{
		int i;

        for (i = 0; i < vtxcount; i++)
		{
			batch.vertices.push_back(vtx[i]);
			batch.vertices[i].pos = (Vec2F::transform(vtx[i].pos, m_curr_matrix));

			if (flip_vertically)
				batch.vertices[i].uv.y = 1.0f - batch.vertices[i].uv.y;

			if (pixel_snap)
			{
				batch.vertices[i].pos.x = calc::floor(batch.vertices[i].pos.x);
				batch.vertices[i].pos.y = calc::floor(batch.vertices[i].pos.y);
			}
		}

		for (i = 0; i < idxcount; i++)
			batch.indices.push_back(idx[i]);
	}

	m_batches.push_back(batch);
}

void SpriteBatch::push_quad(const Quad& quad, const Colour& colour)
{
	Vertex vertices[4];
	u32 indices[6];

	gfxutil::quad(
		vertices, indices,
		quad,
		Quad(RectF::one()),
		colour,
		peek_colour_mode()
	);

	push_vertices(vertices, 4, indices, 6);
}

void SpriteBatch::push_triangle(const Triangle& tri, const Colour& colour)
{
	Vertex vertices[3];
	u32 indices[3];

	gfxutil::tri(
		vertices, indices,
		tri,
		Triangle(
			Vec2F(0.0f, 0.0f),
			Vec2F(0.5f, 1.0f),
			Vec2F(1.0f, 0.0f)
		),
		colour,
		peek_colour_mode()
	);

	push_vertices(vertices, 3, indices, 3);
}

void SpriteBatch::push_texture(const Ref<Texture>& tex, const Colour& colour)
{
	push_texture(tex, RectI(0, 0, tex->size().x, tex->size().y), colour);
}

void SpriteBatch::push_texture(const Ref<Texture>& tex, const RectI& source, const Colour& colour)
{
	push_colour_mode(COLOUR_MODE_NORMAL);

	set_texture(tex);

	Vertex vertices[4];
	u32 indices[6];

	gfxutil::quad(
		vertices, indices,
		Quad(source),
		Quad(source) / tex->size(),
		colour,
		peek_colour_mode()
	);

	push_vertices(vertices, 4, indices, 6, tex->framebuffer_parent() && bknd::Renderer::inst()->properties().origin_bottom_left);

	pop_colour_mode();
}

void SpriteBatch::push_string(
	const char* str,
	const Ref<Font>& font,
	const Colour& colour
)
{
	push_string(str, font, [&](Font::Character c, int idx) -> Vec2F { return Vec2F::zero(); }, colour);
}

void SpriteBatch::push_string(
	const char* str,
	const Ref<Font>& font,
	const std::function<Vec2F(Font::Character,int)>& offset_fn,
	const Colour& colour
)
{
	// todo: new line support

	const auto& atlas = font->atlas();

	push_colour_mode(COLOUR_MODE_RED_ONLY);

	float cursor_x = 0.0f;

	for (int i = 0; i < cstr::length(str); i++)
	{
		auto c = font->character(str[i]);

		push_matrix(Mat3x2::create_translation(
			Vec2F(cursor_x + c.draw_offset.x, c.draw_offset.y) +
			offset_fn(c, i)
		));

		push_texture(atlas.texture, c.bbox, colour);

		pop_matrix();

		cursor_x +=
			c.advance_x +
			font->kern_advance(str[i], str[i + 1]);
	}

	pop_colour_mode();
}

void SpriteBatch::push_circle(const Circle& circle, u32 accuracy, const Colour& colour)
{
	push_colour_mode(COLOUR_MODE_SILHOUETTE);

	float dtheta = calc::TAU / accuracy;

	for (float theta = 0.0f; theta < calc::TAU; theta += dtheta)
	{
		Triangle triangle;
		triangle.a = circle.position;
		triangle.b = Vec2F::from_angle(theta         , circle.radius) + circle.position;
		triangle.c = Vec2F::from_angle(theta + dtheta, circle.radius) + circle.position;

		push_triangle(triangle, colour);
	}

	pop_colour_mode();
}

void SpriteBatch::push_line(const Line& line, float thickness, const Colour& colour)
{
	push_colour_mode(COLOUR_MODE_SILHOUETTE);

	Vec2F direction = line.direction();
	Vec2F perp = direction.perpendicular();
	Vec2F pos0 = line.a + perp * thickness * 0.5f;
	Vec2F pos1 = line.b + perp * thickness * 0.5f;
	Vec2F pos2 = line.b - perp * thickness * 0.5f;
	Vec2F pos3 = line.a - perp * thickness * 0.5f;

	push_quad(Quad(pos0, pos1, pos2, pos3), colour);

	pop_colour_mode();
}

void SpriteBatch::set_texture(const Ref<Texture>& tex)
{
	m_curr_batch.texture = tex;
}

void SpriteBatch::set_sampler(const TextureSampler& sampler)
{
	m_curr_batch.sampler = sampler;
}

void SpriteBatch::reset_texture()
{
	m_curr_batch.texture = nullptr;
}

Ref<Texture> SpriteBatch::peek_texture() const
{
	return m_curr_batch.texture;
}

const TextureSampler& SpriteBatch::peek_sampler() const
{
	return m_curr_batch.sampler;
}

void SpriteBatch::set_shader(const Ref<Shader>& shd)
{
	m_curr_batch.shader = shd;
}

void SpriteBatch::reset_shader()
{
	m_curr_batch.shader = nullptr;
}

Ref<Shader> SpriteBatch::peek_shader() const
{
	return m_curr_batch.shader;
}

void SpriteBatch::push_material(const Ref<Material>& material)
{
	m_material_stack.push_back(material);
	m_curr_batch.material = material;
}

Ref<Material> SpriteBatch::pop_material()
{
	auto result = m_material_stack.pop_back();
	m_curr_batch.material = m_material_stack.back();
	return result;
}

Ref<Material> SpriteBatch::peek_material() const
{
	if (m_material_stack.size() <= 0)
		return nullptr;

	return m_curr_batch.material;
}

void SpriteBatch::push_stencil(Compare stencil)
{
	m_stencil_stack.push_back(stencil);
	m_curr_batch.stencil = stencil;
}

Compare SpriteBatch::pop_stencil()
{
	auto result = m_stencil_stack.pop_back();
	m_curr_batch.stencil = m_stencil_stack.back();
	return result;
}

const Compare& SpriteBatch::peek_stencil() const
{
	if (m_stencil_stack.size() <= 0)
		return Compare::none();

	return m_stencil_stack.back();
}

void SpriteBatch::push_depth(CompareFunc depth)
{
	m_depth_stack.push_back(depth);
	m_curr_batch.depth = depth;
}

CompareFunc SpriteBatch::pop_depth()
{
	auto result = m_depth_stack.pop_back();
	m_curr_batch.depth = m_depth_stack.back();
	return result;
}

CompareFunc SpriteBatch::peek_depth() const
{
	if (m_depth_stack.size() <= 0)
		return COMPARE_NONE;

	return m_depth_stack.back();
}

void SpriteBatch::push_scissor(const RectI& scissor)
{
	m_scissor_stack.push_back(scissor);
	m_curr_batch.scissor = scissor;
}

RectI SpriteBatch::pop_scissor()
{
	auto result = m_scissor_stack.pop_back();
	m_curr_batch.scissor = m_scissor_stack.back();
	return result;
}

const RectI& SpriteBatch::peek_scissor() const
{
	if (m_blend_stack.size() <= 0)
		return RectI::zero();

	return m_scissor_stack.back();
}

void SpriteBatch::push_viewport(const RectI& viewport)
{
	m_viewport_stack.push_back(viewport);
	m_curr_batch.viewport = viewport;
}

RectI SpriteBatch::pop_viewport()
{
	auto result = m_viewport_stack.pop_back();
	m_curr_batch.viewport = m_viewport_stack.back();
	return result;
}

const RectI& SpriteBatch::peek_viewport() const
{
	if (m_viewport_stack.size() <= 0)
		return RectI::zero();

	return m_viewport_stack.back();
}

void SpriteBatch::push_layer(float layer)
{
	m_layer_stack.push_back(layer);
	m_curr_batch.layer = layer;
}

float SpriteBatch::pop_layer()
{
	auto result = m_layer_stack.pop_back();
	m_curr_batch.layer = m_layer_stack.back();
	return result;
}

float SpriteBatch::peek_layer() const
{
	if (m_layer_stack.size() <= 0)
		return 0.0f;

	return m_layer_stack.back();
}

void SpriteBatch::push_matrix(const Mat3x2& matrix)
{
	m_matrix_stack.push_back(m_curr_matrix);
	m_curr_matrix = matrix * m_curr_matrix;
}

Mat3x2 SpriteBatch::pop_matrix()
{
	auto val = m_curr_matrix;
	m_curr_matrix = m_matrix_stack.pop_back();
	return val;
}

const Mat3x2& SpriteBatch::peek_matrix() const
{
	return m_curr_matrix;
}

void SpriteBatch::push_blend(const BlendMode& blend)
{
	m_blend_stack.push_back(blend);
	m_curr_batch.blend = blend;
}

BlendMode SpriteBatch::pop_blend()
{
	auto result = m_blend_stack.pop_back();
	m_curr_batch.blend = m_blend_stack.back();
	return result;
}

const BlendMode& SpriteBatch::peek_blend() const
{
	if (m_blend_stack.size() <= 0)
		return BlendMode::generic();

	return m_blend_stack.back();
}

void SpriteBatch::push_colour_mode(ColourMode mode)
{
	m_colour_mode_stack.push_back(mode);
}

ColourMode SpriteBatch::pop_colour_mode()
{
	return m_colour_mode_stack.pop_back();
}

ColourMode SpriteBatch::peek_colour_mode() const
{
	if (m_colour_mode_stack.size() <= 0)
		return COLOUR_MODE_NORMAL;

	return m_colour_mode_stack.back();
}
