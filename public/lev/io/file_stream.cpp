#include <lev/io/file_stream.h>
#include <backend/system.h>

using namespace lv;

FileStream::FileStream()
	: Stream()
{
}

FileStream::FileStream(const char* filename, const char* mode)
	: Stream()
{
	open(filename, mode);
}

FileStream& FileStream::open(const char* filename, const char* mode)
{
	p_stream = bknd::System::inst()->stream_from_file(filename, mode);
	return *this;
}

bool FileStream::get_line(String& str, s32& seek_cache)
{
	str.clear();

	char c;

	do
	{
		read(&c, 1);
		str.push_back(c);

		seek_cache++;

		if (seek_cache > size())
			return false;

		seek(seek_cache);
	}
	while (c != '\n');

	return true;
}
