#include <lev/io/memory_stream.h>
#include <backend/system.h>

// todo i havent tested this im just going off of intuition here this could very well cause an atomic blast (epic style (((tm))))

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
	p_stream = System::stream_from_memory(memory, length);
	return *this;
}
