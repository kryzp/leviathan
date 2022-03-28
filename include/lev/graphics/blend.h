#pragma once

namespace lev
{
	enum BlendEquation
	{
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN,
		MAX
	};

	enum BlendFunc
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
		BlendEquation equation_rgb;
		BlendEquation equation_alpha;
		
		BlendFunc func_src_rgb;
		BlendFunc func_dst_rgb;

		BlendFunc func_src_alpha;
		BlendFunc func_dst_alpha;
	};
}
