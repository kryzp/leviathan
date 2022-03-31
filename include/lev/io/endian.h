#pragma once

namespace lev
{
	enum Endianness
	{
		ENDIANNESS_LITTLE,
		ENDIANNESS_BIG
	};

	namespace Endian
	{
		Endianness get_endianness();

		bool is_endian(Endianness endian);
		bool is_little_endian();
		bool is_big_endian();
	};
}
