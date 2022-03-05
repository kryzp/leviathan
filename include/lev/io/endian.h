#pragma once

namespace lev::io
{
	enum class Endianness
	{
		LITTLE,
		BIG
	};

	namespace Endian
	{
		Endianness get_endianness();

		bool is_endian(Endianness endian);
		bool is_little_endian();
		bool is_big_endian();
	};
}
