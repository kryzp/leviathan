#include <lev/graphics/gfxutil.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/core/util.h>

using namespace lv;

static void get_vtx_mode(u8 mode, float* buf)
{
    buf[0] = (mode & COLOUR_MODE_NORMAL) != 0;
    buf[1] = (mode & COLOUR_MODE_ALPHA_ONLY) != 0;
    buf[2] = (mode & COLOUR_MODE_RED_ONLY) != 0;
    buf[3] = (mode & COLOUR_MODE_SILHOUETTE) != 0;
}

void GfxUtil::quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour colour, u8 mode)
{
    colour.premultiply();

	float vtxmode[4];
    get_vtx_mode(mode, vtxmode);

	float rr = (float)colour.r / 255.0f;
	float gg = (float)colour.g / 255.0f;
	float bb = (float)colour.b / 255.0f;
	float aa = (float)colour.a / 255.0f;

	Vertex arrvtx[4] = {
		{ .pos = quad.a, .uv = uv.a, .col = { rr, gg, bb, aa } },
		{ .pos = quad.b, .uv = uv.b, .col = { rr, gg, bb, aa } },
		{ .pos = quad.c, .uv = uv.c, .col = { rr, gg, bb, aa } },
		{ .pos = quad.d, .uv = uv.d, .col = { rr, gg, bb, aa } }
    };

	get_vtx_mode(mode, arrvtx[0].mode);
	get_vtx_mode(mode, arrvtx[1].mode);
	get_vtx_mode(mode, arrvtx[2].mode);
	get_vtx_mode(mode, arrvtx[3].mode);

    static const u32 arrindices[6] = {
        0, 1, 2,
        0, 2, 3
	};
    
    mem::copy(vtx, arrvtx, sizeof(arrvtx));
    mem::copy(indices, arrindices, sizeof(arrindices));
}

void GfxUtil::tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour colour, u8 mode)
{
    colour.premultiply();

	float rr = (float)colour.r / 255.0f;
	float gg = (float)colour.g / 255.0f;
	float bb = (float)colour.b / 255.0f;
	float aa = (float)colour.a / 255.0f;

	Vertex arrvtx[3] = {
		{ .pos = triangle.a, .uv = uv.a, .col = { rr, gg, bb, aa }},
		{ .pos = triangle.b, .uv = uv.b, .col = { rr, gg, bb, aa }},
		{ .pos = triangle.c, .uv = uv.c, .col = { rr, gg, bb, aa }}
    };

	get_vtx_mode(mode, arrvtx[0].mode);
	get_vtx_mode(mode, arrvtx[1].mode);
	get_vtx_mode(mode, arrvtx[2].mode);

    static const u32 arrindices[3] = {
        0, 1, 2
    };

    mem::copy(vtx, arrvtx, sizeof(arrvtx));
    mem::copy(indices, arrindices, sizeof(arrindices));
}
