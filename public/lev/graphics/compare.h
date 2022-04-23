#pragma once

#include <lev/core/util.h>

namespace lev
{
	enum CompareFace
	{
		LEV_FACE_NONE = 0,
		LEV_FACE_FRONT,
		LEV_FACE_BACK,
		LEV_FACE_FRONT_AND_BACK,
		LEV_FACE_MAX
	};

	enum CompareFunc
	{
		LEV_COMPARE_NONE = 0,
		LEV_COMPARE_NEVER,
		LEV_COMPARE_LESS,
		LEV_COMPARE_LEQUAL,
		LEV_COMPARE_GREATER,
		LEV_COMPARE_GEQUAL,
		LEV_COMPARE_EQUAL,
		LEV_COMPARE_NOTEQUAL,
		LEV_COMPARE_ALWAYS,
		LEV_COMPARE_MAX
	};

	enum CompareFail
	{
		LEV_COMPARE_FAIL_NONE = 0,
		LEV_COMPARE_FAIL_KEEP,
		LEV_COMPARE_FAIL_ZERO,
		LEV_COMPARE_FAIL_REPLACE,
		LEV_COMPARE_FAIL_INCR,
		LEV_COMPARE_FAIL_INCR_WRAP,
		LEV_COMPARE_FAIL_DECR,
		LEV_COMPARE_FAIL_DECR_WRAP,
		LEV_COMPARE_FAIL_INVERT,
		LEV_COMPARE_FAIL_MAX
	};

	struct Compare
	{
		u8 face;

		u8 func;
		s32 ref;
		u32 mask;

		u8 s_fail;
		u8 dp_fail;
		u8 dp_pass;

		Compare()
			: face(LEV_FACE_NONE)
			, func(LEV_COMPARE_NONE)
			, ref(0)
			, mask(0)
			, s_fail(LEV_COMPARE_FAIL_NONE)
			, dp_fail(LEV_COMPARE_FAIL_NONE)
			, dp_pass(LEV_COMPARE_FAIL_NONE)
		{
		}

		Compare(
			u8 face,
			u8 func,
			s32 ref,
			u32 mask,
			u8 s_fail,
			u8 dp_fail,
			u8 dp_pass
		)
			: face(face)
			, func(func)
			, ref(ref)
			, mask(mask)
			, s_fail(s_fail)
			, dp_fail(dp_fail)
			, dp_pass(dp_pass)
		{
		}

		static const Compare& none();
	};
}
