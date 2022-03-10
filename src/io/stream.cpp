#include <lev/io/stream.h>
#include <backend/system.h>

using namespace lev;
using namespace lev::io;

Stream::Stream()
	: p_stream(nullptr)
{
}

void Stream::read(void* buffer, s64 length)
{
	System::stream_read(p_stream, buffer, length);
}

void Stream::write(void* data, s64 length)
{
	System::stream_write(p_stream, data, length);
}

void Stream::seek(s64 offset)
{
	System::stream_seek(p_stream, offset);
}

s64 Stream::position()
{
	return System::stream_position(p_stream);
}

s64 Stream::length()
{
	return System::stream_length(p_stream);
}

void Stream::close()
{
	System::stream_close(p_stream);
}
