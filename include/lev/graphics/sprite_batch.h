#pragma once

#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/graphics/framebuffer.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/math/vec2.h>
#include <lev/math/colour.h>
#include <lev/containers/vector.h>

namespace lev { struct RenderPass; }

namespace lev::gfx
{
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void initialize();

		void render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer = nullptr);

		void render_texture(const TextureRegion& tex);
		void render_texture(const Ref<Texture>& tex);

		void push_matrix(const Mat3x2& mat);
		Mat3x2 pop_matrix();
		const Mat3x2& peek_matrix() const;

		void push_blend(const BlendMode& blend);
		BlendMode pop_blend();
		const BlendMode& peek_blend() const;

		void push_shader(const Ref<Shader>& shader);
		Ref<Shader> pop_shader();
		Ref<Shader> peek_shader();

	private:
		Ref<Shader> m_default_shader;
		BlendMode m_default_blend;

		struct Vertex
		{
			Vec2 pos;
			Colour col;
			Vec2 texcoord;
		};

		struct RenderBatch
		{
			Ref<Material> material;
			Mat3x2 matrix;
			BlendMode blend;
		};

		void render_batch(RenderPass& pass, const RenderBatch& b);

		Vector<Mat3x2> m_matrix_stack;
		Mat3x2 m_transform_matrix;

		Vector<Ref<Shader>> m_shader_stack;
		Vector<BlendMode> m_blend_stack;

		Vector<RenderBatch> m_batches;
	};
}
