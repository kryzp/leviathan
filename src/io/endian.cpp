#include <lev/io/endian.h>
#include <lev/core/util.h>

using namespace lev;
using namespace lev::io;

Endianness Endian::get_endianness()
{
	Endianness result = Endianness::LITTLE;
	if (is_big_endian())
		result = Endianness::BIG;
	return result;
}

bool Endian::is_endian(Endianness endian)
{
	return (
		(endian == Endianness::LITTLE && is_little_endian()) ||
		(endian == Endianness::BIG && is_big_endian())
	);
}

bool Endian::is_little_endian() { return *((u16*)"AB") == 0x4241; }
bool Endian::is_big_endian() { return *((u16*)"AB") == 0x4142; }
