#pragma once

#include <lev/core/util.h>
#include <lev/math/rect.h>
#include <lev/graphics/material.h>
#include <lev/graphics/mesh.h>
#include <lev/graphics/render_target.h>
#include <lev/graphics/blend.h>
#include <lev/graphics/compare.h>

namespace lev
{
	struct RenderPass
	{
		Ref<Material> material;
		Ref<Mesh> mesh;
		Ref<RenderTarget> target;
		BlendMode blend;
		Compare stencil;
		CompareFunc depth;
		RectI viewport;
		RectI scissor;
		int instance_count;

		void perform() const;
	};
}
