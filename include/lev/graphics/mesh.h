#pragma once

#include <lev/core/util.h>
#include <lev/math/colour.h>
#include <lev/math/vec2.h>
#include <lev/graphics/gfxutil.h>

namespace lev::gfx
{
	class Mesh : public NonCopyable, public NonMovable
	{
	public:
		~Mesh() = default;

		static Ref<Mesh> create();

		virtual void vertex_data(const Vertex* vertices, u64 count) = 0;
		virtual void index_data(const u32* indices, u64 count) = 0;

		virtual u64 vertex_count() const = 0;
		virtual u64 index_count() const = 0;
	};
}
