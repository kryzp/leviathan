#pragma once

namespace Lev
{
	enum class Endianness
	{
		Little,
		Big
	};

	namespace Endian
	{
		Endianness get_endianness();

		bool is_endian(Endianness endian);
		bool is_little_endian();
		bool is_big_endian();
	};
}
