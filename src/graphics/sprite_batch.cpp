#include <lev/graphics/sprite_batch.h>
#include <lev/graphics/mesh.h>
#include <lev/containers/string.h>
#include <backend/renderer.h>

using namespace lev;
using namespace lev::gfx;

SpriteBatch::SpriteBatch()
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::render(const Mat4x4& proj)
{
	String projectionname = current_shader()->get_uniform_data(UniformFlags::PROJECTION).name;
	m_shader_stack.back()->set(projectionname, proj);

	for (auto& b : m_batches)
		render_batch(b);

	m_batches.clear();
}

void SpriteBatch::render_batch(const RenderBatch& b)
{
	float width = 1.0f;
	float height = 1.0f;

	Ref<Material> material = create_ref<Material>();
	Ref<Mesh> mesh = Renderer::create_mesh();

	material->shader(current_shader());

	if (b.texture)
	{
		width = b.texture->data().width;
		height = b.texture->data().height;

		material->texture(b.texture);
		material->sampler(b.sampler);
	}

	Vertex vertices[] = {
		// todo: make colours controllable somehow
		// todo: also very temporary while i get stuff working
		{ .pos = Vec2(0.0f,  0.0f),   .texcoord = Vec2(0.0f, 0.0f), .col = Colour::WHITE },
		{ .pos = Vec2(0.0f,  height), .texcoord = Vec2(0.0f, 1.0f), .col = Colour::WHITE },
		{ .pos = Vec2(width, height), .texcoord = Vec2(1.0f, 1.0f), .col = Colour::WHITE },
		{ .pos = Vec2(width, 0.0f),   .texcoord = Vec2(1.0f, 0.0f), .col = Colour::WHITE }
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

	Renderer::render({
		.mesh = mesh,
		.material = material
	});
}

void SpriteBatch::render_texture(const TextureRegion& tex)
{
	// todo: temp
	render_texture(tex.texture);
}

void SpriteBatch::render_texture(const Ref<Texture>& tex)
{
	RenderBatch b = {
		.texture = tex,
		.sampler = TextureSampler(TextureFilter::NEAREST, TextureWrap::CLAMP, TextureWrap::CLAMP),
		.matrix = m_transform_matrix
	};

	m_batches.push_back(b);
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

void SpriteBatch::push_shader(const Ref<Shader>& shader)
{
	m_shader_stack.push_back(shader);
}

Ref<Shader> SpriteBatch::pop_shader()
{
	return m_shader_stack.pop_back();
}

const Ref<Shader>& SpriteBatch::current_shader() const
{
	return m_shader_stack.back();
}
