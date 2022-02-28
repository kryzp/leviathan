#include <lev/graphics/sprite_batch.h>
#include <lev/containers/string.h>
#include <backend/renderer.h>

using namespace Lev;

SpriteBatch::SpriteBatch()
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::render(const Mat4x4& projection)
{
	String projectionname = current_shader()->get_uniform_data(UniformFlags::Projection).name;
	m_shader_stack.back()->set("u_projection", projection);

	for (auto& b : m_batches)
		render_batch(b);

	m_batches.clear();
}

void SpriteBatch::render_batch(const RenderBatch& b)
{
	float width = 1.0f;
	float height = 1.0f;

	current_shader()->use();

	if (b.texture)
	{
		String textureuniform = current_shader()->get_uniform_data(UniformFlags::MainTexture).name;
		m_shader_stack.back()->set(textureuniform, 0);
		b.texture->bind();
		
		width = b.texture->data().width;
		height = b.texture->data().height;
	}

	Vertex vertices[] = {
		{ .pos = Vec2(width, height), .col = Colour::RED,    .texcoord = Vec2(1.0f, 1.0f) },
		{ .pos = Vec2(width, 0.0f),   .col = Colour::GREEN,  .texcoord = Vec2(1.0f, 0.0f) },
		{ .pos = Vec2(0.0f,  0.0f),   .col = Colour::BLUE,   .texcoord = Vec2(0.0f, 0.0f) },
		{ .pos = Vec2(0.0f,  height), .col = Colour::YELLOW, .texcoord = Vec2(0.0f, 1.0f) }
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

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	RenderPass pass = {
		.vertices = glvertices,
		.vertex_count = 32,
		.indices = indices,
		.index_count = 6
	};

	Renderer::render(pass);
}

void SpriteBatch::render_texture(const Ref<Texture>& tex)
{
	RenderBatch b = {
		.texture = tex,
		.sampler = TextureSampler(TextureFilter::Point, TextureWrap::Clamp, TextureWrap::Clamp),
		.matrix = m_transform_matrix
	};

	m_batches.push_back(b);
}

void SpriteBatch::compute_matrix()
{
	Mat3x2 result = Mat3x2::IDENTITY;

	for (const auto& mat : m_matrix_stack)
		result *= mat;

	m_transform_matrix = result;
}

void SpriteBatch::push_matrix(const Mat3x2& matrix)
{
	m_matrix_stack.push_back(matrix);
	compute_matrix();
}

Mat3x2 SpriteBatch::pop_matrix()
{
	auto val = m_matrix_stack.pop_back();
	compute_matrix();
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
