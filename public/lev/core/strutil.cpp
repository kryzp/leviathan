#include <lev/core/util.h>
#include <string.h>

using namespace lev;

u64 StrUtil::length(const char* str)
{
	return ::strlen(str);
}

char* StrUtil::cncat(char* dst, const char* src, u64 size)
{
	return ::strncat(dst, src, size);
}

char* StrUtil::copy(char* dst, const char* src, u64 size)
{
	return ::strncpy(dst, src, size);
}

int StrUtil::compare(const char* str1, const char* str2)
{
	return ::strcmp(str1, str2);
}

u64 StrUtil::spn(const char* str, const char* control)
{
	return ::strcspn(str, control);
}

void StrUtil::fromint(char* buf, s32 value, int radix)
{
	_itoa(value, buf, radix);
}

void StrUtil::fromint64(char* buf, s64 value, int radix)
{
	_i64toa(value, buf, radix);
}
