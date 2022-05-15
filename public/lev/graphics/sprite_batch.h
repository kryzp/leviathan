#pragma once

// y e s
#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/graphics/font.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/render_target.h>
#include <lev/graphics/compare.h>
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

namespace lv
{
	struct RenderPass;

	struct Vertex
	{
		Vec2F pos;
		Vec2F uv;
		float col[4];
		float mode[4];
	};

	enum SpriteSort
	{
		SPRITE_SORT_NONE = 0,
		SPRITE_SORT_FTB, // front to back - drawn in front to back order of layer
		SPRITE_SORT_BTF, // back to front - drawn in back to front order of layers
		SPRITE_SORT_DEFERRED, // deferred - drawn in order of render calls individually
		SPRITE_SORT_MAX
	};

    enum ColourMode
    {
        COLOUR_MODE_NONE,
        COLOUR_MODE_NORMAL     = 1 << 0,
        COLOUR_MODE_ALPHA_ONLY = 1 << 1,
        COLOUR_MODE_RED_ONLY   = 1 << 2,
        COLOUR_MODE_SILHOUETTE = 1 << 3,
        COLOUR_MODE_MAX
    };

	// hamburgjer spellign #2
	using ColorMode = ColourMode;

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch() = default;

		void render(const Ref<RenderTarget>& target = nullptr, SpriteSort sort_mode = SPRITE_SORT_FTB);
		void render(const Mat4x4& proj, const Ref<RenderTarget>& target = nullptr, SpriteSort sort_mode = SPRITE_SORT_FTB);
		void clear();

		///////////////////////////////////////////////////////
		//  W E L C O M E   T O   T H E   P U S H   Z O N E  // (kind of a vibe ngl)
		///////////////////////////////////////////////////////

		void push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount, bool flip_vertically = false);
		void push_texture(const Ref<Texture>& tex, const Colour& colour = Colour::white());
		void push_texture(const Ref<Texture>& tex, const RectI& source, const Colour& colour = Colour::white());
		void push_quad(const Quad& quad, const Colour& colour = Colour::white());
		void push_triangle(const Triangle& tri, const Colour& colour = Colour::white());
		void push_string(const char* str, const Ref<Font>& font, const Colour& colour = Colour::white());
		void push_string(const char* str, const Ref<Font>& font, const std::function<Vec2F(Font::Character,int)>& offset_fn, const Colour& colour = Colour::white());
		void push_circle(const Circle& circle, u32 accuracy = 40U, const Colour& colour = Colour::white());
		void push_line(const Line& line, float thickness, const Colour& colour = Colour::white());

		///////////////////////////////////////////////////////

		// texture
		void set_texture(const Ref<Texture>& tex);
		Ref<Texture> peek_texture() const;

		// sampler
		void set_sampler(const TextureSampler& sampler);
		const TextureSampler& peek_sampler() const;

		// shader
		void set_shader(const Ref<Shader>& shd);
		void reset_shader();
		Ref<Shader> peek_shader() const;

		// material
		void push_material(const Ref<Material>& material);
		Ref<Material> pop_material();
		Ref<Material> peek_material() const;

		// stencil
		void push_stencil(Compare stencil);
		Compare pop_stencil();
		const Compare& peek_stencil() const;

		// depth
		void push_depth(CompareFunc depth);
		CompareFunc pop_depth();
		CompareFunc peek_depth() const;

		// scissor
		void push_scissor(const RectI& scissor);
		RectI pop_scissor();
		const RectI& peek_scissor() const;

		// viewport
		void push_viewport(const RectI& viewport);
		RectI pop_viewport();
		const RectI& peek_viewport() const;

		// layer
		void push_layer(float layer);
		float pop_layer();
		float peek_layer() const;

		// matrix
		void push_matrix(const Mat3x2& mat);
		Mat3x2 pop_matrix();
		const Mat3x2& peek_matrix() const;

		// blend
		void push_blend(const BlendMode& blend);
		BlendMode pop_blend();
		const BlendMode& peek_blend() const;

		// colour mode
		void push_colour_mode(ColourMode mode);
		ColourMode pop_colour_mode();
		ColourMode peek_colour_mode() const;

		// snaps all drawing coords to integers - useful in pixel perfect rendering (esp. pixel art games !)
		bool pixel_snap = false;

	private:
		struct RenderBatch
		{
			Ref<Material> material;
			Ref<Shader> shader;
			Ref<Texture> texture;
			TextureSampler sampler;

			CompareFunc depth;
			Compare stencil;

			BlendMode blend;
			float layer;

			RectI viewport;
			RectI scissor;

			Vector<Vertex> vertices;
			Vector<u32> indices;
		};

		static Ref<Material> m_default_material;
		static RenderBatch m_empty_batch;

		Ref<Mesh> m_mesh;

		void initialize();
		bool m_initialized;

		void render_batch(RenderPass& pass, const RenderBatch& b, const Mat4x4& proj);

		RenderBatch m_curr_batch;
		Vector<RenderBatch> m_batches;

		Mat3x2 m_curr_matrix;
		Vector<Mat3x2> m_matrix_stack;

		Vector<float> m_layer_stack;
		Vector<Ref<Material>> m_material_stack;
		Vector<BlendMode> m_blend_stack;
		Vector<CompareFunc> m_depth_stack;
		Vector<Compare> m_stencil_stack;
		Vector<RectI> m_scissor_stack;
		Vector<RectI> m_viewport_stack;
		Vector<ColourMode> m_colour_mode_stack;
	};
}
