#include <lev/core/util.h>

using namespace lev;

void* mem::set(void* ptr, byte val, u64 size)
{
	return ::memset(ptr, val, size);
}

void* mem::copy(void* dst, const void* src, u64 size)
{
	return ::memcpy(dst, src, size);
}

void* mem::move(void* dst, const void* src, u64 size)
{
	return ::memmove(dst, src, size);
}

void* mem::chr(void* ptr, byte val, u64 size)
{
	return ::memchr(ptr, val, size);
}

int mem::compare(const void* p1, const void* p2, u64 size)
{
	return ::memcmp(p1, p2, size);
}

bool mem::vcompare(void* ptr, byte val, u64 size)
{
	byte* mem = (byte*)ptr;
    return (
		(*mem) == val &&
		memcmp(mem, mem + 1, size - 1) == 0
	);
}
