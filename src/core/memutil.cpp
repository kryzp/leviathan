#include <lev/core/util.h>

using namespace lev;

void* MemUtil::alloc(size_t size)
{
	return malloc(size);
}

void* MemUtil::alloc_zero(size_t size)
{
	void* ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void* MemUtil::set(void* ptr, s32 val, size_t size)
{
	return memset(ptr, val, size);
}

void* MemUtil::set_zero(void* ptr, size_t size)
{
	return memset(ptr, 0, size);
}

void* MemUtil::copy(void* dst, const void* src, size_t size)
{
	return memcpy(dst, src, size);
}

void* MemUtil::move(void* dst, const void* src, size_t size)
{
	return memmove(dst, src, size);
}

void* MemUtil::chr(void* ptr, i32 val, size_t size)
{
	return memchr(ptr, val, size);
}

int MemUtil::compare(const void* p1, const void* p2, size_t size)
{
	return memcmp(p1, p2, size);
}
