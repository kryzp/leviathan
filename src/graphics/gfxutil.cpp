#include <lev/graphics/gfxutil.h>
#include <lev/core/util.h>

using namespace lev;

void GfxUtil::quad(Vertex* vtx, u32* indices, const Quad& quad, Colour colour)
{
    colour.premultiply();

    static const Quad uv = Quad(
        Vec2(0.0f, 1.0f),
        Vec2(0.0f, 0.0f),
        Vec2(1.0f, 0.0f),
        Vec2(1.0f, 1.0f)
    );

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

void GfxUtil::quad(Vertex* vtx, u32* indices, float x, float y, float width, float height, Colour colour)
{
    Quad quad = Quad(
        Vec2(x, y),
        Vec2(x, height),
        Vec2(width, height),
        Vec2(width, y)
    );

    static const Quad uv = Quad(
        Vec2(0.0f, 1.0f),
        Vec2(0.0f, 0.0f),
        Vec2(1.0f, 0.0f),
        Vec2(1.0f, 1.0f)
    );

    GfxUtil::quad(vtx, indices, quad, colour);
}

void GfxUtil::tri(Vertex* vtx, u32* indices, const Triangle& triangle, Colour colour)
{
    colour.premultiply();

    static const Triangle uv = Triangle(
        Vec2(0.5f, 1.0f),
        Vec2(0.0f, 0.0f),
        Vec2(1.0f, 0.0f)
    );

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
