#pragma once

#include <lev/io/stream.h>

namespace lev::io
{
	class MemoryStream : public Stream
	{
	public:
		MemoryStream();
		MemoryStream(void* memory, u64 length);
		void open(void* memory, u64 length);
	};
}
