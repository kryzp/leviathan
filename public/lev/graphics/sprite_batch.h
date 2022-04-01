#pragma once

// y e s
#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/graphics/font.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/framebuffer.h>
#include <lev/math/mat3x2.h>
#include <lev/math/mat4x4.h>
#include <lev/math/vec2.h>
#include <lev/math/triangle.h>
#include <lev/math/quad.h>
#include <lev/math/circle.h>
#include <lev/math/line.h>
#include <lev/math/colour.h>
#include <lev/containers/vector.h>

#include <functional>

namespace lev
{
	// todo: different sprite sort / rendering modes (front to back, back to front, immediate, etc...)

	struct RenderPass;

	enum TextAlign
	{
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTRE,
		TEXT_ALIGN_RIGHT
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		
		void render(const Ref<Framebuffer>& framebuffer = nullptr);
		void render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer = nullptr);

		///////////////////////////////////////////////////////
		//  W E L C O M E   T O   T H E   P U S H   Z O N E  // (kind of a vibe ngl)
		///////////////////////////////////////////////////////

		void push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount);
		void push_texture(const TextureRegion& tex, const Colour& colour = Colour::white(), const Colour& mode = Colour(1.0f, 0.0f, 0.0f, 0.0f));
		void push_texture(const Ref<Texture>& tex, const Colour& colour = Colour::white(), const Colour& mode = Colour(1.0f, 0.0f, 0.0f, 0.0f));
		void push_quad(const Quad& quad, const Colour& colour = Colour::white(), const Colour& mode = Colour(1.0f, 0.0f, 0.0f, 0.0f));
		void push_triangle(const Triangle& tri, const Colour& colour = Colour::white(), const Colour& mode = Colour(1.0f, 0.0f, 0.0f, 0.0f));
		void push_text(const char* str, const Font& font, TextAlign align = TEXT_ALIGN_LEFT, const Colour& colour = Colour::white());
		void push_text(const char* str, const Font& font, const std::function<Vec2F(Font::Character,int)>& offsetfn, TextAlign align = TEXT_ALIGN_LEFT, const Colour& colour = Colour::white());
		void push_circle(const Circle& circle, u32 accuracy = 40U, const Colour& colour = Colour::white());
		void push_line(const Line& line, float thickness, const Colour& colour = Colour::white());

		///////////////////////////////////////////////////////

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
