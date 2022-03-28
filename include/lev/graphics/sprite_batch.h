#pragma once

#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/math/mat3x2.h>
#include <lev/math/vec2.h>
#include <lev/math/colour.h>
#include <lev/containers/vector.h>

#include <functional>

namespace lev
{
	// todo: different sprite sort / rendering modes (front to back, back to front, immediate, etc...)

	struct Mat4x4;
	struct RenderPass;
	class Framebuffer;
	class Font;
	class Mesh;
	struct Quad;
	struct Triangle;
	struct Vertex;
	struct FontCharacter;

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		
		void render(const Ref<Framebuffer>& framebuffer = nullptr);
		void render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer = nullptr);

		void push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount);
		void push_quad(const Quad& quad, const Colour& colour = Colour::white());
		void push_triangle(const Triangle& tri, const Colour& colour = Colour::white());
		void push_texture(const TextureRegion& tex, const Colour& colour = Colour::white());
		void push_texture(const Ref<Texture>& tex, const Colour& colour = Colour::white());
		void push_string(const char* str, const Ref<Font>& font, const Colour& colour = Colour::white());
		void push_string(const char* str, const Ref<Font>& font, const std::function<Vec2F(FontCharacter,int)>& offsetfn, const Colour& colour = Colour::white());

		void set_texture(const Ref<Texture>& tex, int idx = 0);
		void set_sampler(const TextureSampler& sampler, int idx = 0);
		void reset_texture(int idx = 0);

		Ref<Texture> peek_texture(int idx = 0);
		const TextureSampler& peek_sampler(int idx = 0);

		void set_shader(const Ref<Shader>& shader);
		void reset_shader();
		void push_shader(const Ref<Shader>& shader);
		void pop_shader();
		Ref<Shader> peek_shader();

		// todo: layers

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

		Ref<Shader> m_font_shader;

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
