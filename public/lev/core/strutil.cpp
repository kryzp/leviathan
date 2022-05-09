#include <lev/core/util.h>
#include <string.h>

#include <cctype>

using namespace lv;

u64 cstr::length(const char* str)
{
	return ::strlen(str);
}

char* cstr::cncat(char* dst, const char* src, u64 size)
{
	return ::strncat(dst, src, size);
}

char* cstr::copy(char* dst, const char* src, u64 size)
{
	return ::strncpy(dst, src, size);
}

int cstr::compare(const char* str1, const char* str2)
{
	return ::strcmp(str1, str2);
}

u64 cstr::cspan(const char* scan, const char* match)
{
	return ::strcspn(scan, match);
}

char* cstr::token(char* str, const char* delimiter)
{
	return ::strtok(str, delimiter);
}

bool cstr::is_space(int c)
{
	return ::isspace(c);
}

char cstr::to_upper(int c)
{
	return ::toupper(c);
}

char cstr::to_lower(int c)
{
	return ::tolower(c);
}

void cstr::from_int(char* buf, s32 value, int radix)
{
	_itoa(value, buf, radix);
}

void cstr::from_int64(char* buf, s64 value, int radix)
{
	_i64toa(value, buf, radix);
}

int cstr::to_int(const char* str)
{
	return atoi(str);
}

float cstr::to_float(const char* str)
{
	return atof(str);
}
