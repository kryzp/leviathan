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
		Vec2 pos;
		Vec2 uv;
		Colour col;
	};

	namespace GfxUtil
	{
		void quad(Vertex* vtx, u32* indices, const Quad& quad, Colour colour);
		void quad(Vertex* vtx, u32* indices, float x, float y, float width, float height, Colour colour);
		void tri(Vertex* vtx, u32* indices, const Triangle& triangle, Colour colour);
	}
};
