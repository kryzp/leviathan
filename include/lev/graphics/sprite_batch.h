#pragma once

#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/math/vec2.h>
#include <lev/math/colour.h>
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

		void push_shader(const Ref<Shader>& shader);
		Ref<Shader> pop_shader();
		const Ref<Shader>& current_shader() const;

		void render(const Mat4x4& proj);

		void render_texture(const TextureRegion& tex);
		void render_texture(const Ref<Texture>& tex);

	private:
		struct Vertex
		{
			Vec2 pos;
			Colour col;
			Vec2 texcoord;
		};

		struct RenderBatch
		{
			Ref<Texture> texture;
			TextureSampler sampler;
			Mat3x2 matrix;
		};

		void render_batch(const RenderBatch& b);

		Vector<Mat3x2> m_matrix_stack;
		Mat3x2 m_transform_matrix;

		Vector<Ref<Shader>> m_shader_stack;

		Vector<RenderBatch> m_batches;
	};
}
