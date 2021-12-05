#pragma once

#include <inttypes.h>
#include <memory>

#ifdef LEV_DEBUG

// writes to null memory crashing the program at that location
// neat trick!

#define LEV_ASSERT(exp) do{if(!(exp)){*((volatile int*)0)=0;}}while(false)

#else

#define LEV_ASSERT(exp)

#endif

#define LEV_ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((*arr)))

#define LEV_BYTES(x)     (x)
#define LEV_KILOBYTES(x) (LEV_BYTES(x) * 1024LL)
#define LEV_MEGABYTES(x) (LEV_KILOBYTES(x) * 1024LL)
#define LEV_GIGABYTES(x) (LEV_MEGABYTES(x) * 1024LL)
#define LEV_TERABYTES(x) (LEV_GIGABYTES(x) * 1024LL)

namespace Lev
{
	using u8  = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	
	using s8  = int8_t;
	using s16 = int16_t;
	using s32 = int32_t;
	using s64 = int64_t;

	using i8  = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using c8  = char;
	using c16 = char16_t;
	using c32 = char32_t;

	using sbyte = char;
	using ubyte = unsigned char;
	using byte = unsigned char;

	using f32 = float;
	using f64 = double;

	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Scope<T> create_scope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> create_ref(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}
