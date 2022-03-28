#include <lev/io/file_stream.h>
#include <backend/system.h>

using namespace lev;

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
	p_stream = System::stream_from_file(filename, mode);
	return *this;
}
