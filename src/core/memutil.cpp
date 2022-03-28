#include <lev/core/util.h>

using namespace lev;

void* MemUtil::set(void* ptr, s32 val, u64 size)
{
	return ::memset(ptr, val, size);
}

void* MemUtil::clear(void* ptr, u64 size)
{
	return ::memset(ptr, 0, size);
}

void* MemUtil::copy(void* dst, const void* src, u64 size)
{
	return ::memcpy(dst, src, size);
}

void* MemUtil::move(void* dst, const void* src, u64 size)
{
	return ::memmove(dst, src, size);
}

void* MemUtil::chr(void* ptr, s32 val, u64 size)
{
	return ::memchr(ptr, val, size);
}

int MemUtil::compare(const void* p1, const void* p2, u64 size)
{
	return ::memcmp(p1, p2, size);
}
