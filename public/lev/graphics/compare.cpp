#include <lev/graphics/compare.h>

using namespace lev;

const Compare& Compare::none()
{
	static const Compare NONE = Compare(
		LEV_FACE_NONE,
		LEV_COMPARE_NONE,
		0, 0,
		LEV_COMPARE_FAIL_NONE,
		LEV_COMPARE_FAIL_NONE,
		LEV_COMPARE_FAIL_NONE
	);

	return NONE;
}
