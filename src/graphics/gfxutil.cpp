#include <lev/graphics/gfxutil.h>
#include <lev/core/util.h>

using namespace lev;

void GfxUtil::quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour)
{
    colour.premultiply();

	Vertex arrvtx[4] = {
		{ .pos = quad.a, .uv = uv.a, .col = { colour.r, colour.g, colour.b } },
		{ .pos = quad.b, .uv = uv.b, .col = { colour.r, colour.g, colour.b } },
		{ .pos = quad.c, .uv = uv.c, .col = { colour.r, colour.g, colour.b } },
		{ .pos = quad.d, .uv = uv.d, .col = { colour.r, colour.g, colour.b } }
    };

    static const u32 arrindices[6] = {
        0, 1, 3,
        1, 2, 3
    };

    if (vtx)
        MemUtil::copy(vtx, arrvtx, sizeof(arrvtx));

    if (indices)
        MemUtil::copy(indices, arrindices, sizeof(arrindices));
}

void GfxUtil::tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour colour)
{
    colour.premultiply();

	Vertex arrvtx[3] = {
		{ .pos = triangle.a, .uv = uv.a, .col = { colour.r, colour.g, colour.b } },
		{ .pos = triangle.b, .uv = uv.b, .col = { colour.r, colour.g, colour.b } },
		{ .pos = triangle.c, .uv = uv.c, .col = { colour.r, colour.g, colour.b } }
    };

    static const u32 arrindices[6] = {
        0, 1, 2
    };

    if (vtx)
        MemUtil::copy(vtx, arrvtx, sizeof(arrvtx));

    if (indices)
        MemUtil::copy(indices, arrindices, sizeof(arrindices));
}