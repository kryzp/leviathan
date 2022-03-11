#pragma once

#include <lev/core/util.h>
#include <lev/io/stream.h>

namespace lev::io
{
	class FileStream : public Stream
	{
	public:
		FileStream();
		FileStream(const char* filename, const char* mode);
		void open(const char* filename, const char* mode);
	};
}
