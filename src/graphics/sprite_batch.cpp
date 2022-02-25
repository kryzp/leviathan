#include <lev/graphics/sprite_batch.h>
#include <backend/graphics.h>

using namespace Lev;

SpriteBatch::SpriteBatch()
{
	// todo
}

SpriteBatch::~SpriteBatch()
{
	// todo
}

void SpriteBatch::push_matrix(const Mat3x2& matrix)
{
	m_transform_stack.push_back(matrix);
}

Mat3x2 SpriteBatch::pop_matrix()
{
	return m_transform_stack.pop_back();
}

void SpriteBatch::set_shader(const Ref<Shader>& shader)
{
	// todo
}

void SpriteBatch::render_vertices(float* vertices, int vertex_count, u32* indices, int index_count)
{
	Graphics::render({
		.vertices = vertices,
		.vertex_count = vertex_count,
		.indices = indices,
		.index_count = index_count
	});
}

void SpriteBatch::render_quad(int x, int y)
{
	f32 vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};

	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	render_vertices(vertices, LEV_ARRAY_LENGTH(vertices), indices, LEV_ARRAY_LENGTH(indices));
}

Mat3x2 SpriteBatch::final_transform() const
{
	Mat3x2 result = Mat3x2::IDENTITY;

	for (const auto& mat : m_transform_stack)
		result *= mat;

	return result;
}
