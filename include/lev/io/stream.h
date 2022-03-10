#pragma once

#include <lev/core/non_copyable.h>
#include <lev/core/util.h>

namespace lev::io
{
	class Stream : public NonCopyable
	{
	public:
		Stream();

		virtual void read(void* buffer, s64 length);
		virtual void write(void* data, s64 length);
		virtual void seek(s64 offset);
		virtual s64 position();
		virtual s64 length();
		virtual void close();

	protected:
		void* p_stream;
	};
}
