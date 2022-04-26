#include <lev/core/util.h>
#include <string.h>

#include <cctype>

using namespace lev;

u64 str::length(const char* str)
{
	return ::strlen(str);
}

char* str::cncat(char* dst, const char* src, u64 size)
{
	return ::strncat(dst, src, size);
}

char* str::copy(char* dst, const char* src, u64 size)
{
	return ::strncpy(dst, src, size);
}

int str::compare(const char* str1, const char* str2)
{
	return ::strcmp(str1, str2);
}

u64 str::cspan(const char* scan, const char* match)
{
	return ::strcspn(scan, match);
}

bool str::is_space(int c)
{
	return ::isspace(c);
}

void str::fromint(char* buf, s32 value, int radix)
{
	_itoa(value, buf, radix);
}

void str::fromint64(char* buf, s64 value, int radix)
{
	_i64toa(value, buf, radix);
}
