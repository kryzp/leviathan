#pragma once

// todo: probably get rid of all of these header files and put them in the .cpp file ...
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
#include <lev/containers/hash_map.h>
#include <lev/containers/vector.h>
#include <lev/containers/string.h>

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

		void quad();

		void push_matrix(const Mat3x2& mat);
		Mat3x2 pop_matrix();
		const Mat3x2& peek_matrix() const;

		void push_material(const Material& material);
		Material pop_material();
		Material& peek_material();
		Ref<Shader> peek_shader();

		void push_blend(const BlendMode& blend);
		BlendMode pop_blend();
		BlendMode& peek_blend();

	private:
		struct RenderBatch
		{
			Ref<Mesh> mesh;
			Material material;
			BlendMode blend;
		};

		Ref<Shader> m_default_shader;

		void initialize();
		void render_batch(RenderPass& pass, const RenderBatch& b);

		Vector<Mat3x2> m_matrix_stack;
		Mat3x2 m_transform_matrix;
		Vector<BlendMode> m_blend_stack;
		Vector<Material> m_material_stack;
		Vector<RenderBatch> m_batches;
	};
}
