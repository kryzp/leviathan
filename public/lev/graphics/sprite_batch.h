#pragma once

// y e s
#include <lev/graphics/blend.h>
#include <lev/graphics/material.h>
#include <lev/graphics/font.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/framebuffer.h>
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

namespace lev
{
	struct RenderPass;

	enum SpriteSort
	{
		SPRITE_SORT_FTB, // front to back - drawn in front to back order of layer
		SPRITE_SORT_BTF, // back to front - drawn in back to front order of layers
		SPRITE_SORT_DEFERRED, // deferred - drawn in order of render calls individually
		SPRITE_SORT_MAX
	};

	enum TextAlign
	{
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTRE,
		TEXT_ALIGN_RIGHT
	};

    enum SBFragMode
    {
        SB_FRAGMODE_NONE,
        SB_FRAGMODE_ALL        = 1 << 0,
        SB_FRAGMODE_ALPHA      = 1 << 1,
        SB_FRAGMODE_RED        = 1 << 2,
        SB_FRAGMODE_SILHOUETTE = 1 << 3,
        SB_FRAGMODE_MAX
    };

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch() = default;

		void render(const Ref<Framebuffer>& framebuffer = nullptr, int sort_mode = SPRITE_SORT_FTB);
		void render(const Mat4x4& proj, const Ref<Framebuffer>& framebuffer = nullptr, int sort_mode = SPRITE_SORT_FTB);

		///////////////////////////////////////////////////////
		//  W E L C O M E   T O   T H E   P U S H   Z O N E  // (kind of a vibe ngl)
		///////////////////////////////////////////////////////

		void push_vertices(const Vertex* vtx, u64 vtxcount, const u32* idx, u64 idxcount);
		void push_texture(const TextureRegion& tex, const Colour& colour = Colour::white(), u8 mode = SB_FRAGMODE_ALL);
		void push_texture(const Ref<Texture>& tex, const Colour& colour = Colour::white(), u8 mode = SB_FRAGMODE_ALL);
		void push_quad(const Quad& quad, const Colour& colour = Colour::white(), u8 mode = SB_FRAGMODE_ALL);
		void push_triangle(const Triangle& tri, const Colour& colour = Colour::white(), u8 mode = SB_FRAGMODE_ALL);
		void push_string(const char* str, const Ref<Font>& font, TextAlign align = TEXT_ALIGN_LEFT, const Colour& colour = Colour::white());
		void push_string(const char* str, const Ref<Font>& font, const std::function<Vec2F(Font::Character,int)>& offsetfn, TextAlign align = TEXT_ALIGN_LEFT, const Colour& colour = Colour::white());
		void push_circle(const Circle& circle, u32 accuracy = 40U, const Colour& colour = Colour::white());
		void push_line(const Line& line, float thickness, const Colour& colour = Colour::white());

		///////////////////////////////////////////////////////

		void set_texture(const Ref<Texture>& tex, int idx = 0);
		void set_sampler(const TextureSampler& sampler, int idx = 0);
		void reset_texture(int idx = 0);

		Ref<Texture> peek_texture(int idx = 0);
		const TextureSampler& peek_sampler(int idx = 0);

		void push_stencil(Compare stencil);
		Compare pop_stencil();
		Compare& peek_stencil();

		void push_depth(u8 depth);
		u8 pop_depth();
		u8& peek_depth();

		void push_scissor(const RectI& scissor);
		RectI pop_scissor();
		RectI& peek_scissor();

		void push_viewport(const RectI& viewport);
		RectI pop_viewport();
		RectI& peek_viewport();

		void push_layer(float layer);
		float pop_layer();
		float& peek_layer();

		void push_matrix(const Mat3x2& mat);
		Mat3x2 pop_matrix();
		Mat3x2& peek_matrix();

		void push_material(const Material& material);
		Material pop_material();
		Material& peek_material();

		void push_blend(const BlendMode& blend);
		BlendMode pop_blend();
		BlendMode& peek_blend();

	private:
		struct RenderBatch
		{
			Ref<Mesh> mesh;
			Material material;
			BlendMode blend;
			u8 depth;
			Compare stencil;
			RectI viewport;
			RectI scissor;
			float layer;
		};

		void initialize();
		bool m_initialized;

		void render_batch(RenderPass& pass, const RenderBatch& b);

		Vector<RenderBatch> m_batches;

		Vector<Mat3x2> m_matrix_stack;
		Mat3x2 m_transform_matrix;

		// todo: convert these into arrays?
		Vector<float> m_layer_stack;
		Vector<Material> m_material_stack;
		Vector<BlendMode> m_blend_stack;
		Vector<u8> m_depth_stack;
		Vector<Compare> m_stencil_stack;
		Vector<RectI> m_scissor_stack;
		Vector<RectI> m_viewport_stack;
	};
}
