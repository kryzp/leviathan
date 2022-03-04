#pragma once

#include <lev/core/util.h>
#include <lev/core/non_copyable.h>
#include <lev/core/non_movable.h>
#include <lev/math/colour.h>
#include <lev/math/vec2.h>

namespace Lev
{
	enum class VertexAttrib
	{
		NONE,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MAX
	};

	struct VertexFormat
	{
		int attrib_count;
		VertexAttrib attribs[8];
		int stride;
	};

	class Mesh : public NonCopyable, public NonMovable
	{
	public:
		~Mesh() = default;

		virtual void vertex_data(const void* vertices, u64 count, const VertexFormat& format) = 0;
		virtual void index_data(const u32* indices, u64 count) = 0;

		virtual u64 vertex_count() const = 0;
		virtual u64 index_count() const = 0;
		virtual VertexFormat format() const = 0;
	};
}
