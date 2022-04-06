#include <lev/io/memory_stream.h>
#include <backend/system.h>

using namespace lev;

MemoryStream::MemoryStream()
	: Stream()
{
}

MemoryStream::MemoryStream(void* memory, u64 length)
{
	open(memory, length);
}

MemoryStream& MemoryStream::open(void* memory, u64 length)
{
	p_stream = System::inst()->stream_from_memory(memory, length);
	return *this;
}

/////////////////////////////////////////////////////////

ConstMemoryStream::ConstMemoryStream()
	: Stream()
{
}

ConstMemoryStream::ConstMemoryStream(const void* memory, u64 length)
{
	open(memory, length);
}

ConstMemoryStream& ConstMemoryStream::open(const void* memory, u64 length)
{
	p_stream = System::inst()->stream_from_const_memory(memory, length);
	return *this;
}
