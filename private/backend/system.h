#pragma once

#include <lev/core/util.h>

namespace lev { struct Config; }

namespace lev::bknd
{
	struct SystemProperties
	{
		int temp;
	};

	class System : public NonCopyable, public NonMovable
	{
	public:
		System() = default;
		virtual ~System() = default;

        static System* inst();

		virtual SystemProperties properties() = 0;

		virtual bool init(const Config& cfg) = 0;
		virtual void postinit() = 0;
		virtual void destroy() = 0;

		virtual void update() = 0;
		virtual void present() = 0;

		virtual void set_window_position(int x, int y) = 0;
		virtual void get_window_position(int* x, int* y) = 0;

		virtual int window_width() = 0;
		virtual int window_height() = 0;

		virtual int draw_width() = 0;
		virtual int draw_height() = 0;

		virtual void show_cursor(bool toggle) = 0;
		virtual bool cursor_visible() = 0;

		virtual void sleep(u32 ms) = 0;
		virtual u64 ticks() = 0;

		virtual void* stream_from_file(const char* filepath, const char* mode) = 0;
		virtual void* stream_from_memory(void* memory, s64 size) = 0;
		virtual void* stream_from_const_memory(const void* memory, s64 size) = 0;
		virtual s64 stream_read(void* stream, void* ptr, s64 size) = 0;
		virtual s64 stream_write(void* stream, const void* ptr, s64 size) = 0;
		virtual s64 stream_seek(void* stream, s64 offset) = 0;
		virtual s64 stream_size(void* stream) = 0;
		virtual s64 stream_position(void* stream) = 0;
		virtual void stream_close(void* stream) = 0;

		virtual void* gl_context_create() = 0;
		virtual void gl_context_make_current(void* context) = 0;
		virtual void gl_context_destroy(void* context) = 0;
		virtual bool gl_load_glad_loader() = 0;
	};
}
