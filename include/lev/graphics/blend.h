#pragma once

namespace lev::gfx
{
	enum BlendFunction
	{
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN,
		MAX
	};

	enum BlendFactor
	{
		ZERO,
		ONE,
		SRC_COLOUR,
		SRC1_COLOUR,
		ONE_MINUS_SRC_COLOUR,
		ONE_MINUS_SRC1_COLOUR,
		DST_COLOUR,
		ONE_MINUS_DST_COLOUR,
		SRC_ALPHA,
		SRC1_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		ONE_MINUS_SRC1_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOUR,
		ONE_MINUS_CONSTANT_COLOUR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA
	};

	struct BlendMode
	{
		BlendFunction func;
		BlendFactor factor_src;
		BlendFactor factor_dst;
	};
}
