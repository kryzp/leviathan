#pragma once

#include <lev/core/util.h>

namespace lev
{
	class Stream : public NonCopyable
	{
	public:
		Stream();
		virtual ~Stream();

		virtual void read(void* buffer, s64 length) const;
		virtual void write(void* data, s64 length) const;
		virtual void seek(s64 offset) const;
		virtual void close();
		virtual s64 position() const;
		virtual s64 size() const;

		void* stream();
		const void* stream() const;

	protected:
		void* p_stream;
	};
}
