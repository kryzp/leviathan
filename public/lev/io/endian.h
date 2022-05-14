#pragma once

#include <lev/core/util.h>

namespace lv
{
	enum Endianness
	{
		ENDIANNESS_NONE = 0,
		ENDIANNESS_LITTLE,
		ENDIANNESS_BIG,
		ENDIANNESS_MAX
	};

	namespace Endian
	{
		Endianness get_endianness();

		bool is_endian(Endianness endian);
		bool is_little_endian();
		bool is_big_endian();
	};
}
