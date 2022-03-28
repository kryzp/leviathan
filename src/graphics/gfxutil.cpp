#include <lev/graphics/gfxutil.h>
#include <lev/core/util.h>

using namespace lev;

// todo: i dont like the way this looks pretty sus

void GfxUtil::quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour)
{
    colour.premultiply();

	Vertex arrvtx[4] = {
		{ .pos = quad.a, .uv = uv.a, .col = colour },
		{ .pos = quad.b, .uv = uv.b, .col = colour },
		{ .pos = quad.c, .uv = uv.c, .col = colour },
		{ .pos = quad.d, .uv = uv.d, .col = colour }
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
		{ .pos = triangle.a, .uv = uv.a, .col = colour },
		{ .pos = triangle.b, .uv = uv.b, .col = colour },
		{ .pos = triangle.c, .uv = uv.c, .col = colour }
    };

    static const u32 arrindices[6] = {
        0, 1, 2
    };

    if (vtx)
        MemUtil::copy(vtx, arrvtx, sizeof(arrvtx));

    if (indices)
        MemUtil::copy(indices, arrindices, sizeof(arrindices));
}
