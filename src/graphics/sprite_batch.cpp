#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/mesh.h>
#include <lev/containers/string.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

namespace
{
}

SpriteBatch::SpriteBatch()
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
		"#version 330 core\nlayout (location = 0) in vec3 a_pos;\nlayout (location = 1) in vec3 a_colour;\nlayout (location = 2) in vec2 a_tex_coord;\nout vec3 in_colour;\nout vec2 tex_coord;\nuniform mat4 u_projection;\nvoid main() {\ngl_Position = u_projection * vec4(a_pos, 1.0);\nin_colour = a_colour;\ntex_coord = a_tex_coord;\n}",
		"#version 330 core\nout vec4 frag_colour;\nin vec3 in_colour;\nin vec2 tex_coord;\nuniform sampler2D u_texture;\nvoid main() {\nfrag_colour = texture(u_texture, tex_coord) * vec4(in_colour, 1.0);\n}",
		true
	);

	m_default_shader->assign_uniform("u_projection", gfx::UniformType::MAT4X4, gfx::UniformFlags::PROJECTION);
	m_default_shader->assign_uniform("u_texture", gfx::UniformType::SAMPLER2D, gfx::UniformFlags::MAIN_TEXTURE);
}

void SpriteBatch::render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer)
{
	String projectionname = peek_shader()->uniform_name(UniformFlags::PROJECTION);
	peek_shader()->set(projectionname, proj);

	RenderPass pass = {
		.target = framebuffer
	};

	for (auto& b : m_batches)
		render_batch(pass, b);

	m_batches.clear();
}

void SpriteBatch::render_batch(RenderPass& pass, const RenderBatch& b)
{
	pass.blend = b.blend;
	pass.material = b.material;

	// temp: create a quad
	Ref<Mesh> mesh = Mesh::create();
	{
		float width = 1.0f;
		float height = 1.0f;

		if (b.material->texture)
		{
			width = b.material->texture->width();
			height = b.material->texture->height();
		}

		Vertex vertices[] = {
			// todo: make colours controllable somehow
			// todo: also very temporary while i get stuff working
			{ .pos = Vec2(0.0f,  0.0f),   .col = Colour::WHITE, .texcoord = Vec2(0.0f, 0.0f) },
			{ .pos = Vec2(width, 0.0f),   .col = Colour::WHITE, .texcoord = Vec2(1.0f, 0.0f) },
			{ .pos = Vec2(width, height), .col = Colour::WHITE, .texcoord = Vec2(1.0f, 1.0f) },
			{ .pos = Vec2(0.0f,  height), .col = Colour::WHITE, .texcoord = Vec2(0.0f, 1.0f) }
		};

		u32 indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		f32 glvertices[32];

		for (int i = 0; i < 4; i++)
		{
			Vertex vertex = vertices[i];

			vertex.pos = Vec2::transform(vertex.pos, b.matrix);
	
			glvertices[i*8 + 0] = vertex.pos.x;
			glvertices[i*8 + 1] = vertex.pos.y;
			glvertices[i*8 + 2] = 0.0f;
	
			glvertices[i*8 + 3] = vertex.col.r / 255.0f;
			glvertices[i*8 + 4] = vertex.col.g / 255.0f;
			glvertices[i*8 + 5] = vertex.col.b / 255.0f;
	
			glvertices[i*8 + 6] = vertex.texcoord.x;
			glvertices[i*8 + 7] = vertex.texcoord.y;
		}

		mesh->index_data(indices, 6);
		mesh->vertex_data(glvertices, 32, {
			.attrib_count = 3,
			.attribs = {
				VertexAttrib::FLOAT3, // position
				VertexAttrib::FLOAT3, // colour
				VertexAttrib::FLOAT2  // texture coordinates
			},
			.stride = 8
		});
	}

	pass.mesh = mesh;

	Renderer::render(pass);
}

void SpriteBatch::render_texture(const TextureRegion& tex)
{
	// todo: temp
	render_texture(tex.texture);
}

void SpriteBatch::render_texture(const Ref<Texture>& tex)
{
	RenderBatch batch;
	{
		batch.material = create_ref<Material>();
		batch.material->shader = peek_shader();
		batch.material->texture = tex;
		batch.material->sampler = {
			.filter = TextureFilter::NEAREST,
			.wrap_x = TextureWrap::CLAMP,
			.wrap_y = TextureWrap::CLAMP
		};
		batch.blend = peek_blend();
		batch.matrix = m_transform_matrix;
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
