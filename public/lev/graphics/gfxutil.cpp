#include <lev/graphics/gfxutil.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/core/util.h>

using namespace lev;

static Colour get_vtx_mode(u8 mode)
{
    Colour c = 0;
    c.r = (mode & SB_FRAGMODE_ALL) != 0;
    c.g = (mode & SB_FRAGMODE_ALPHA) != 0;
    c.b = (mode & SB_FRAGMODE_RED) != 0;
    c.a = (mode & SB_FRAGMODE_SILHOUETTE) != 0;
    return c;
}

void GfxUtil::quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour, u8 mode)
{
    colour.premultiply();

    auto vtxmode = get_vtx_mode(mode);

	Vertex arrvtx[4] = {
		{ .pos = quad.a, .uv = uv.a, .col = colour, .mode = vtxmode },
		{ .pos = quad.b, .uv = uv.b, .col = colour, .mode = vtxmode },
		{ .pos = quad.c, .uv = uv.c, .col = colour, .mode = vtxmode },
		{ .pos = quad.d, .uv = uv.d, .col = colour, .mode = vtxmode }
    };

    static const u32 arrindices[6] = {
        0, 1, 3,
        1, 2, 3
    };
    
    mem::copy(vtx, arrvtx, sizeof(arrvtx));
    mem::copy(indices, arrindices, sizeof(arrindices));
}

void GfxUtil::tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour colour, u8 mode)
{
    colour.premultiply();

    auto vtxmode = get_vtx_mode(mode);

	Vertex arrvtx[3] = {
		{ .pos = triangle.a, .uv = uv.a, .col = colour, .mode = vtxmode },
		{ .pos = triangle.b, .uv = uv.b, .col = colour, .mode = vtxmode },
		{ .pos = triangle.c, .uv = uv.c, .col = colour, .mode = vtxmode }
    };

    static const u32 arrindices[3] = {
        0, 1, 2
    };

    mem::copy(vtx, arrvtx, sizeof(arrvtx));
    mem::copy(indices, arrindices, sizeof(arrindices));
}
