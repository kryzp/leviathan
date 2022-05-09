#include <lev/io/stream.h>
#include <backend/system.h>

using namespace lv;

Stream::Stream()
	: p_stream(nullptr)
{
}

Stream::~Stream()
{
	if (p_stream)
		bknd::System::inst()->stream_close(p_stream);
}

Stream& Stream::read(void* buffer, s64 length)
{
	bknd::System::inst()->stream_read(p_stream, buffer, length);
	return *this;
}

Stream& Stream::write(void* data, s64 length)
{
	bknd::System::inst()->stream_write(p_stream, data, length);
	return *this;
}

Stream& Stream::seek(s64 offset)
{
	bknd::System::inst()->stream_seek(p_stream, offset);
	return *this;
}

Stream& Stream::close()
{
	bknd::System::inst()->stream_close(p_stream);
	p_stream = nullptr;
	return *this;
}

s64 Stream::position()
{
	return bknd::System::inst()->stream_position(p_stream);
}

s64 Stream::size()
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
