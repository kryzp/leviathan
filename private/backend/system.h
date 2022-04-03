#pragma once

#include <lev/core/util.h>

namespace lev
{
	struct Config;

	namespace System
	{
		bool init(const Config* cfg);
		void destroy();
		void postinit();
		void update();
		void present();

		void set_window_position(int x, int y);
		void get_window_position(int* x, int* y);

		int window_width();
		int window_height();

		int draw_width();
		int draw_height();

		void sleep(u32 ms);
		u64 ticks();

		void* stream_from_file(const char* filepath, const char* mode);
		void* stream_from_memory(void* memory, s64 size);
		void* stream_from_const_memory(const void* memory, s64 size);
		s64 stream_read(void* stream, void* ptr, s64 size);
		s64 stream_write(void* stream, const void* ptr, s64 size);
		s64 stream_seek(void* stream, s64 offset);
		s64 stream_size(void* stream);
		s64 stream_position(void* stream);
		void stream_close(void* stream);

		void* gl_context_create();
		void gl_context_make_current(void* context);
		void gl_context_destroy(void* context);

		bool gl_load_glad_loader();
	}
}
