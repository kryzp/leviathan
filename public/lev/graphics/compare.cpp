#include <lev/graphics/compare.h>

using namespace lev;

const Compare& Compare::none()
{
	static const Compare NONE = Compare(
		FACE_NONE,
		COMPARE_NONE,
		0, 0,
		COMPARE_FAIL_NONE,
		COMPARE_FAIL_NONE,
		COMPARE_FAIL_NONE
	);

	return NONE;
}
