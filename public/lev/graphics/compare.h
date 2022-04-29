#pragma once

#include <lev/core/util.h>

namespace lev
{
	enum CompareFace
	{
		FACE_NONE = 0,
		FACE_FRONT,
		FACE_BACK,
		FACE_FRONT_AND_BACK,
		FACE_MAX
	};

	enum CompareFunc
	{
		COMPARE_NONE = 0,
		COMPARE_NEVER,
		COMPARE_LESS,
		COMPARE_LEQUAL,
		COMPARE_GREATER,
		COMPARE_GEQUAL,
		COMPARE_EQUAL,
		COMPARE_NOTEQUAL,
		COMPARE_ALWAYS,
		COMPARE_MAX
	};

	enum CompareFail
	{
		COMPARE_FAIL_NONE = 0,
		COMPARE_FAIL_KEEP,
		COMPARE_FAIL_ZERO,
		COMPARE_FAIL_REPLACE,
		COMPARE_FAIL_INCR,
		COMPARE_FAIL_INCR_WRAP,
		COMPARE_FAIL_DECR,
		COMPARE_FAIL_DECR_WRAP,
		COMPARE_FAIL_INVERT,
		COMPARE_FAIL_MAX
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
			: face(FACE_NONE)
			, func(COMPARE_NONE)
			, ref(0)
			, mask(0)
			, s_fail(COMPARE_FAIL_NONE)
			, dp_fail(COMPARE_FAIL_NONE)
			, dp_pass(COMPARE_FAIL_NONE)
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
