#include <lev/io/endian.h>
#include <lev/core/util.h>

using namespace lv;

Endianness Endian::get_endianness()
{
	return is_big_endian()
		? ENDIANNESS_BIG
		: ENDIANNESS_LITTLE;
}

bool Endian::is_endian(Endianness endian)
{
	return (
		(endian == ENDIANNESS_LITTLE && is_little_endian()) ||
		(endian == ENDIANNESS_BIG && is_big_endian())
	);
}

bool Endian::is_little_endian()	{ return *(reinterpret_cast<const u16*>("AB")) == 0x4241; }
bool Endian::is_big_endian()	{ return *(reinterpret_cast<const u16*>("AB")) == 0x4142; }
