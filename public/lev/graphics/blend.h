#pragma once

#include <lev/core/util.h>

namespace lv
{
	enum BlendEquation
	{
		BLEND_EQUATION_NONE = 0,
		BLEND_EQUATION_ADD,
		BLEND_EQUATION_SUBTRACT,
		BLEND_EQUATION_REVERSE_SUBTRACT,
		BLEND_EQUATION_MIN,
		BLEND_EQUATION_MAX
	};

	enum BlendFunc
	{
		BLEND_FUNC_NONE = 0,
		BLEND_FUNC_ZERO,
		BLEND_FUNC_ONE,
		BLEND_FUNC_SRC_COLOUR,
		BLEND_FUNC_SRC1_COLOUR,
		BLEND_FUNC_ONE_MINUS_SRC_COLOUR,
		BLEND_FUNC_ONE_MINUS_SRC1_COLOUR,
		BLEND_FUNC_DST_COLOUR,
		BLEND_FUNC_ONE_MINUS_DST_COLOUR,
		BLEND_FUNC_SRC_ALPHA,
		BLEND_FUNC_SRC1_ALPHA,
		BLEND_FUNC_ONE_MINUS_SRC_ALPHA,
		BLEND_FUNC_ONE_MINUS_SRC1_ALPHA,
		BLEND_FUNC_DST_ALPHA,
		BLEND_FUNC_ONE_MINUS_DST_ALPHA,
		BLEND_FUNC_CONSTANT_COLOUR,
		BLEND_FUNC_ONE_MINUS_CONSTANT_COLOUR,
		BLEND_FUNC_CONSTANT_ALPHA,
		BLEND_FUNC_ONE_MINUS_CONSTANT_ALPHA,
		BLEND_FUNC_MAX
	};

	struct BlendMode
	{
		BlendEquation equation_rgb;
		BlendEquation equation_alpha;
		
		BlendFunc func_src_rgb;
		BlendFunc func_dst_rgb;

		BlendFunc func_src_alpha;
		BlendFunc func_dst_alpha;

		BlendMode()
			: equation_rgb(BLEND_EQUATION_NONE)
			, equation_alpha(BLEND_EQUATION_NONE)
			, func_src_rgb(BLEND_FUNC_NONE)
			, func_dst_rgb(BLEND_FUNC_NONE)
			, func_src_alpha(BLEND_FUNC_NONE)
			, func_dst_alpha(BLEND_FUNC_NONE)
		{
		}

		BlendMode(
			BlendEquation eq_rgb, BlendEquation eq_alp,
			BlendFunc fn_src_rgb, BlendFunc fn_dst_rgb,
			BlendFunc fn_src_alp, BlendFunc fn_dst_alp
		)
			: equation_rgb(eq_rgb)
			, equation_alpha(eq_alp)
			, func_src_rgb(fn_src_rgb)
			, func_dst_rgb(fn_dst_rgb)
			, func_src_alpha(fn_src_alp)
			, func_dst_alpha(fn_dst_alp)
		{
		}

		static const BlendMode& generic();
		static const BlendMode& none();
	};
}
