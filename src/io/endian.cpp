#include <lev/io/endian.h>
#include <lev/core/util.h>

using namespace Lev;

Endianness Endian::get_endianness()
{
	Endianness result = Endianness::Little;
	if (is_big_endian())
		result = Endianness::Big;
	return result;
}

bool Endian::is_endian(Endianness endian)
{
	return (
		(endian == Endianness::Little && is_little_endian()) ||
		(endian == Endianness::Big && is_big_endian())
	);
}

bool Endian::is_little_endian() { return *((u16*)"AB") == 0x4241; }
bool Endian::is_big_endian() { return *((u16*)"AB") == 0x4142; }
