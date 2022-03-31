#pragma once

#include <lev/io/stream.h>

namespace lev
{
	class MemoryStream : public Stream
	{
	public:
		MemoryStream();
		MemoryStream(void* memory, u64 length);
		MemoryStream& open(void* memory, u64 length);
	};
}
