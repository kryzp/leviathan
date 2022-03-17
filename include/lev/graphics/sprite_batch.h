#pragma once

// todo: probably get rid of all of these header files and put them in the .cpp file ...
#include <lev/graphics/texture.h>
#include <lev/graphics/shader.h>
#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/graphics/mesh.h>
#include <lev/math/mat3x2.h>
#include <lev/math/vec2.h>
#include <lev/math/colour.h>
#include <lev/math/quad.h>
#include <lev/math/triangle.h>
#include <lev/containers/vector.h>

namespace lev { struct Mat4x4; struct RenderPass; }

namespace lev::gfx
{
	class Framebuffer;

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		
		void render(const Ref<Framebuffer>& framebuffer = nullptr);
		void render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer = nullptr);

		void push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount);
		void push_quad(const Colour& colour = Colour::white());
		void push_quad(const Quad& quad, const Colour& colour = Colour::white());
		void push_triangle(const Triangle& tri, const Colour& colour = Colour::white());
		void push_texture(const Ref<Texture>& tex, const TextureSampler& sampler = TextureSampler::pixel(), const Colour& colour = Colour::white());

		void set_texture(const Ref<Texture>& tex, const TextureSampler& sampler = TextureSampler::pixel(), int idx = 0);
		Ref<Texture> peek_texture(int idx = 0);
		const TextureSampler& peek_sampler(int idx = 0);

		void set_shader(const Ref<Shader>& shader);
		void reset_shader();
		Ref<Shader> peek_shader();

		void push_matrix(const Mat3x2& mat);
		Mat3x2 pop_matrix();
		const Mat3x2& peek_matrix() const;

		void push_material(const Material& material);
		Material pop_material();
		const Material& peek_material();

		void push_blend(const BlendMode& blend);
		BlendMode pop_blend();
		const BlendMode& peek_blend();

	private:
		struct RenderBatch
		{
			Ref<Mesh> mesh;
			Material material;
			BlendMode blend;
		};

		Ref<Shader> m_default_shader;

		void initialize();
		bool m_initialized;

		void render_batch(RenderPass& pass, const RenderBatch& b);

		Vector<Mat3x2> m_matrix_stack;
		Mat3x2 m_transform_matrix;
		Vector<BlendMode> m_blend_stack;
		Vector<Material> m_material_stack;
		Vector<RenderBatch> m_batches;
	};
}
