#pragma once

#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/math/mat3x2.h>
#include <lev/containers/vector.h>

namespace Lev
{
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void push_matrix(const Mat3x2& mat);
		Mat3x2 pop_matrix();
		Mat3x2 final_transform() const;

		void set_shader(const Ref<Shader>& shader);

		void render_vertices(float* vertices, int vertex_count, u32* indices, int index_count);
		void render_quad(int x, int y);

	private:
		Vector<Mat3x2> m_transform_stack;
	};
}
