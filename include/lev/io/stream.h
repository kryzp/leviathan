#pragma once

#include <lev/core/util.h>

namespace lev
{
	class Stream : public NonCopyable
	{
	public:
		Stream();
		virtual ~Stream();

		virtual Stream& read(void* buffer, s64 length);
		virtual Stream& write(void* data, s64 length);
		virtual Stream& seek(s64 offset);
		virtual Stream& close();
		virtual s64 position();
		virtual s64 size();

		void* stream();
		const void* stream() const;

	protected:
		void* p_stream;
	};
}
