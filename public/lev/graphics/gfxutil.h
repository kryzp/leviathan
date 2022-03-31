#pragma once

#include <lev/core/util.h>
#include <lev/math/colour.h>
#include <lev/math/vec2.h>
#include <lev/math/quad.h>
#include <lev/math/triangle.h>

namespace lev
{
	struct Vertex
	{
		Vec2F pos;
		Vec2F uv;
		Colour col;
	};

	namespace GfxUtil
	{
		void quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour);
		void tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour colour);
	}
};
