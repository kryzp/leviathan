#pragma once

#include <lev/core/util.h>
#include <lev/io/stream.h>

namespace lev
{
	class FileStream : public Stream
	{
	public:
		FileStream();
		FileStream(const char* filename, const char* mode);
		FileStream& open(const char* filename, const char* mode);
	};
}
