#pragma once

#include <lev/core/util.h>
#include <lev/io/stream.h>
#include <lev/containers/string.h>

namespace lv
{
	class FileStream : public Stream
	{
	public:
		FileStream();
		FileStream(const char* filename, const char* mode);
		FileStream& open(const char* filename, const char* mode);

		bool get_line(String& str, s32& seek_cache);
	};
}
