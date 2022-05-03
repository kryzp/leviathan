#pragma once

#include <inttypes.h>
#include <memory>

#ifdef LEV_DEBUG

// looks hacky but basically i just dereference oob memory and try writing to it, causing a crash
#define LEV_ASSERT(_exp, _msg) if(!(_exp)){::lev::log::error("ASSERTION: " _msg);*((volatile int*)0)=0;}
#define LEV_ERROR(_msg) {::lev::log::error("ERROR: " _msg);*((volatile int*)0)=0;}

#else

#define LEV_ASSERT(_exp, _msg)
#define LEV_ERROR(_msg)

#endif

template <typename T>
inline void __levutils_swap(T& x, T& y) {
	T tmp = x;
	x = y;
	y = tmp;
}

#define LEV_SWAP(_x, _y) __levutils_swap((_x), (_y))

#define LEV_ARRAY_LENGTH(_arr) (sizeof((_arr)) / sizeof((*_arr)))

#define LEV_BYTES(x)     (x)
#define LEV_KILOBYTES(x) (LEV_BYTES(x) * 1024LL)
#define LEV_MEGABYTES(x) (LEV_KILOBYTES(x) * 1024LL)
#define LEV_GIGABYTES(x) (LEV_MEGABYTES(x) * 1024LL)
#define LEV_TERABYTES(x) (LEV_GIGABYTES(x) * 1024LL)

namespace lev
{
	using i8  = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using s8  = int8_t;
	using s16 = int16_t;
	using s32 = int32_t;
	using s64 = int64_t;

	using u8  = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;
	
	using c8  = char;
	using c16 = char16_t;
	using c32 = char32_t;

	using sbyte = char;
	using ubyte = unsigned char;
	using byte  = unsigned char;

	using f32 = float;
	using f64 = double;

	using r32 = float;
	using r64 = double;

	template <typename T>
	using Unique = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Unique<T> create_scope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> create_ref(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

	template <typename T>
	using Weak = std::weak_ptr<T>;

	enum LogType
	{
		LOG_TYPE_NONE = 0,
		LOG_TYPE_NORMAL,
		LOG_TYPE_WARN,
		LOG_TYPE_ERROR,
		LOG_TYPE_FILE,
		LOG_TYPE_MAX
	};

	namespace log
	{
		bool init();
		void destroy();
		void print(const char* fmt, ...);
		void error(const char* fmt, ...);
		void warn(const char* fmt, ...);
	}

	namespace time
	{
		extern float milliseconds;
		extern float prev_milliseconds;
		extern float elapsed;
		extern float prev_elapsed;
		extern float delta;
		extern u64 step_count;
		extern u64 run_loops;
	};

	namespace mem
	{
		void* set(void* ptr, byte val, u64 size);
		void* copy(void* dst, const void* src, u64 size);
		void* move(void* dst, const void* src, u64 size);
		void* chr(void* ptr, byte val, u64 size);
		int compare(const void* p1, const void* p2, u64 size);
		bool vcompare(void* ptr, byte val, u64 size);
	}

	namespace cstr
	{
		u64 length(const char* str);
		char* cncat(char* dst, const char* src, u64 size);
		char* copy(char* dst, const char* src, u64 size);
		int compare(const char* str1, const char* str2);
		u64 cspan(const char* scan, const char* match);
		char* token(char* str, const char* sep);
		bool is_space(int c);
		char to_upper(int c);
		char to_lower(int c);
		void from_int(char* buf, s32 value, int radix);
		void from_int64(char* buf, s64 value, int radix);
		int to_int(const char* str);
		float to_float(const char* str);

		constexpr u64 hash(const char* str)
		{
			u64 hash = 7521;
			for (int i = 0; str[i] != '\0'; i++)
				hash = ((hash << 5) + hash) + str[i];
			return hash;
		}
	}

	class NonCopyable
	{
	public:
		NonCopyable() = default;
		NonCopyable& operator = (const NonCopyable&) = delete;
		NonCopyable(const NonCopyable&) = delete;
	};

	class NonMovable
	{
	public:
		NonMovable() = default;
		NonMovable& operator = (NonCopyable&&) = delete;
		NonMovable(NonCopyable&&) = delete;
	};
}
