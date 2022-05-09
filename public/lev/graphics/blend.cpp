#include <lev/graphics/blend.h>

using namespace lv;

const BlendMode& BlendMode::generic()
{
	static const BlendMode GENERIC = BlendMode(
		BLEND_EQUATION_ADD,
		BLEND_EQUATION_ADD,
		BLEND_FUNC_SRC_ALPHA,
		BLEND_FUNC_ONE_MINUS_SRC_ALPHA,
		BLEND_FUNC_ONE,
		BLEND_FUNC_ONE
	);

	return GENERIC;
}

const BlendMode& BlendMode::none()
{
	static const BlendMode NONE = BlendMode(
		BLEND_EQUATION_NONE,
		BLEND_EQUATION_NONE,
		BLEND_FUNC_NONE,
		BLEND_FUNC_NONE,
		BLEND_FUNC_NONE,
		BLEND_FUNC_NONE
	);

	return NONE;
}
