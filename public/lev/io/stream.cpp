#include <lev/io/stream.h>
#include <backend/system.h>

using namespace lev;

Stream::Stream()
	: p_stream(nullptr)
{
}

Stream::~Stream()
{
	if (p_stream)
		bknd::System::inst()->stream_close(p_stream);
}

void Stream::read(void* buffer, s64 length) const
{
	bknd::System::inst()->stream_read(p_stream, buffer, length);
}

void Stream::write(void* data, s64 length) const
{
	bknd::System::inst()->stream_write(p_stream, data, length);
}

void Stream::seek(s64 offset) const
{
	bknd::System::inst()->stream_seek(p_stream, offset);
}

void Stream::close()
{
	bknd::System::inst()->stream_close(p_stream);
	p_stream = nullptr;
}

s64 Stream::position() const
{
	return bknd::System::inst()->stream_position(p_stream);
}

s64 Stream::size() const
{
	return bknd::System::inst()->stream_size(p_stream);
}

void* Stream::stream()
{
	return p_stream;
}

const void* Stream::stream() const
{
	return p_stream;
}
