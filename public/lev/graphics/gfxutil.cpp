#include <lev/graphics/gfxutil.h>
#include <lev/graphics/sprite_batch.h>
#include <lev/core/util.h>

using namespace lv;

/*
 * TODO: this place is kinda hot garbage
 */

static void _get_vtx_mode(ColourMode mode, float* buf)
{
    buf[0] = (mode & COLOUR_MODE_NORMAL) != 0;
    buf[1] = (mode & COLOUR_MODE_ALPHA_ONLY) != 0;
    buf[2] = (mode & COLOUR_MODE_RED_ONLY) != 0;
    buf[3] = (mode & COLOUR_MODE_SILHOUETTE) != 0;
}

void gfxutil::quad(Vertex* vtx, u32* indices, const Quad& quad, const Quad& uv, Colour c0, Colour c1, Colour c2, Colour c3, ColourMode mode)
{
	c0.premultiply();
	c1.premultiply();
	c2.premultiply();
	c3.premultiply();

	Vertex arrvtx[4] = {
		{ .pos = quad.a, .uv = uv.a, .col = { (float)c0.r / 255.0f, (float)c0.g / 255.0f, (float)c0.b / 255.0f, (float)c0.a / 255.0f } },
		{ .pos = quad.b, .uv = uv.b, .col = { (float)c1.r / 255.0f, (float)c1.g / 255.0f, (float)c1.b / 255.0f, (float)c1.a / 255.0f } },
		{ .pos = quad.c, .uv = uv.c, .col = { (float)c2.r / 255.0f, (float)c2.g / 255.0f, (float)c2.b / 255.0f, (float)c2.a / 255.0f } },
		{ .pos = quad.d, .uv = uv.d, .col = { (float)c3.r / 255.0f, (float)c3.g / 255.0f, (float)c3.b / 255.0f, (float)c3.a / 255.0f } }
    };

	_get_vtx_mode(mode, arrvtx[0].mode);
	_get_vtx_mode(mode, arrvtx[1].mode);
	_get_vtx_mode(mode, arrvtx[2].mode);
	_get_vtx_mode(mode, arrvtx[3].mode);

    static const u32 arrindices[6] = {
        0, 1, 2,
        0, 2, 3
	};
    
    mem::copy(vtx, arrvtx, sizeof(arrvtx));
    mem::copy(indices, arrindices, sizeof(arrindices));
}

void gfxutil::tri(Vertex* vtx, u32* indices, const Triangle& triangle, const Triangle& uv, Colour c0, Colour c1, Colour c2, ColourMode mode)
{
    c0.premultiply();
    c1.premultiply();
    c2.premultiply();

	Vertex arrvtx[3] = {
		{ .pos = triangle.a, .uv = uv.a, .col = { (float)c0.r / 255.0f, (float)c0.g / 255.0f, (float)c0.b / 255.0f, (float)c0.a / 255.0f }},
		{ .pos = triangle.b, .uv = uv.b, .col = { (float)c1.r / 255.0f, (float)c1.g / 255.0f, (float)c1.b / 255.0f, (float)c1.a / 255.0f }},
		{ .pos = triangle.c, .uv = uv.c, .col = { (float)c2.r / 255.0f, (float)c2.g / 255.0f, (float)c2.b / 255.0f, (float)c2.a / 255.0f }}
    };

	_get_vtx_mode(mode, arrvtx[0].mode);
	_get_vtx_mode(mode, arrvtx[1].mode);
	_get_vtx_mode(mode, arrvtx[2].mode);

    static const u32 arrindices[3] = {
        0, 1, 2
    };

    mem::copy(vtx, arrvtx, sizeof(arrvtx));
    mem::copy(indices, arrindices, sizeof(arrindices));
}
