#pragma once

#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/framebuffer.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/math/vec2.h>
#include <lev/math/colour.h>
#include <lev/math/quad.h>
#include <lev/math/triangle.h>
#include <lev/containers/vector.h>

namespace lev { struct RenderPass; }

namespace lev::gfx
{
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		
		void render(const Ref<Framebuffer>& framebuffer = nullptr);
		void render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer = nullptr);

		void texture(const Ref<Texture>& tex);

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
		void initialize();
		bool m_initialized;

		Ref<Shader> m_default_shader;
		BlendMode m_default_blend;

		struct RenderBatch
		{
			// note: i am making a compremise here
			// by adding the mesh *here* it allows me to also make it possible
			// to use an individual shader per texture, tradeoff being that
			// i cannot do instanced rendering, which is much faster, but means
			// you could only use one shader per batch render and it would be applied
			// to the whole render instead of to a specific sprite.
			// maybe ill have to change this in the future, seems fine for now. its a 2d engine
			// it doesnt need to be omega-fast
			// ...
			// ok maybe for particles
			Ref<Mesh> mesh;
			Ref<Material> material;
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
