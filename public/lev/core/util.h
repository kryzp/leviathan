#pragma once

#include <inttypes.h>
#include <memory>

#ifdef LEV_DEBUG

// looks hacky but basically i just dereference oob memory and try writing to it, causing a crash
#define LEV_ASSERT(_exp, _msg) if(!(_exp)){Log::error("ASSERTION: " _msg);*((volatile int*)0)=0;}
#define LEV_ERROR(_msg) {Log::error("ERROR: " _msg);*((volatile int*)0)=0;}

#else

#define LEV_ASSERT(_exp, _msg)
#define LEV_ERROR(_msg)

#endif

template <class T>
inline void __levutils_swap(T& x, T& y) {
	T tmp = x;
	x = y;
	y = tmp;
}

#define LEV_SWAP(_x, _y) __levutils_swap((_x), (_y))

// todo: replace with singleton to inherit from?
// class MySingleton : public lev::Singleton<MySingleton> { ... }

// should be placed at the top of the class
// e.g:
// class MySingleton {
//     LEV_SINGLETON_CLASS(MySingleton)
//
//     public:
//         void do_the_thing();

// then do:
// LEV_INSTANCE(MySingleton).do_the_thing();

#define LEV_INSTANCE(_classname) (_classname::inst())

#define LEV_SINGLETON_CLASS(_classname) \
public: \
	static _classname& inst() \
	{ \
		static _classname* instance = nullptr; \
		if (!instance) { instance = new _classname(); } \
		LEV_ASSERT(instance, "Instance should not be nullptr. How tf did that even happen are you using a potato to store memory????"); \
		return *instance; \
	} \
private:

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
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Scope<T> create_scope(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> create_ref(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

	enum LogType
	{
		LOG_TYPE_NORMAL,
		LOG_TYPE_WARN,
		LOG_TYPE_ERROR,
		LOG_TYPE_FILE
	};

	namespace Log
	{
		bool init();
		void destroy();

		void print(const char* fmt, ...);
		void error(const char* fmt, ...);
		void warn(const char* fmt, ...);

		void file(const char *msg, ...);
		void filenewline();

		void directory(const char* dir);
		const char* directory();
	}

	struct Time
	{
		static float seconds;
		static float prev_seconds;
		static float milliseconds;
		static float prev_milliseconds;
		static float delta;
		static float deltams;
		static u64 frames;
	};

	namespace MemUtil
	{
		void* set(void* ptr, byte val, u64 size);
		void* copy(void* dst, const void* src, u64 size);
		void* move(void* dst, const void* src, u64 size);
		void* chr(void* ptr, byte val, u64 size);
		int compare(const void* p1, const void* p2, u64 size);
		bool vcompare(void* ptr, byte val, u64 size);
	}

	namespace StrUtil
	{
		u64 length(const char* str);
		char* cncat(char* dst, const char* src, u64 size);
		char* copy(char* dst, const char* src, u64 size);
		int compare(const char* str1, const char* str2);
		u64 spn(const char* str, const char* control);
		void fromint(char* buf, s32 value, int radix);
		void fromint64(char* buf, s64 value, int radix);
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