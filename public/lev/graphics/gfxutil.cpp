#include <lev/graphics/gfxutil.h>
#include <lev/core/util.h>

using namespace lev;

void GfxUtil::quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour, const Colour& mode)
{
    colour.premultiply();

	Vertex arrvtx[4] = {
		{ .pos = quad.a, .uv = uv.a, .col = colour, .mode = mode },
		{ .pos = quad.b, .uv = uv.b, .col = colour, .mode = mode },
		{ .pos = quad.c, .uv = uv.c, .col = colour, .mode = mode },
		{ .pos = quad.d, .uv = uv.d, .col = colour, .mode = mode }
    };

    static const u32 arrindices[6] = {
        0, 1, 3,
        1, 2, 3
    };
    
    MemUtil::copy(vtx, arrvtx, sizeof(arrvtx));
    MemUtil::copy(indices, arrindices, sizeof(arrindices));
}

void GfxUtil::tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour colour, const Colour& mode)
{
    colour.premultiply();

	Vertex arrvtx[3] = {
		{ .pos = triangle.a, .uv = uv.a, .col = colour, .mode = mode },
		{ .pos = triangle.b, .uv = uv.b, .col = colour, .mode = mode },
		{ .pos = triangle.c, .uv = uv.c, .col = colour, .mode = mode }
    };

    static const u32 arrindices[3] = {
        0, 1, 2
    };

    MemUtil::copy(vtx, arrvtx, sizeof(arrvtx));
    MemUtil::copy(indices, arrindices, sizeof(arrindices));
}
