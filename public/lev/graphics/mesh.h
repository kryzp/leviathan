#pragma once

#include <lev/core/util.h>

namespace lev
{
	struct Vertex;

	class Mesh : public NonCopyable, public NonMovable
	{
	public:
		virtual ~Mesh() = default;

		static Ref<Mesh> create();

		virtual void vertex_data(const Vertex* vertices, u64 count) = 0;
		virtual void index_data(const u32* indices, u64 count) = 0;
		virtual void instance_data(const Vertex* vertices, const void* instances, u64 count) = 0;

		virtual u64 vertex_count() const = 0;
		virtual u64 index_count() const = 0;
		virtual u64 instance_count() const = 0;
	};
}
