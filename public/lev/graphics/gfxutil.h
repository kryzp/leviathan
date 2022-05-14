#pragma once

#include <lev/core/util.h>
#include <lev/math/colour.h>
#include <lev/math/vec2.h>
#include <lev/math/quad.h>
#include <lev/math/triangle.h>
#include <lev/graphics/sprite_batch.h>

namespace lv
{
	namespace gfxutil
	{
		void quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour, ColourMode mode);
		void tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour colour, ColourMode mode);
	}
};
