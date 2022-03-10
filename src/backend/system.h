#pragma once

#include <lev/core/util.h>

namespace lev
{
	struct AppConfig;

	namespace System
	{
		bool init(const AppConfig* cfg);
		void destroy();
		void postinit();
		void update();
		void present();

		int window_width();
		int window_height();

		int draw_width();
		int draw_height();

		u64 ticks();

		void* stream_from_file(const char* filepath, const char* mode);
		s64 stream_read(void* stream, void* ptr, s64 size);
		s64 stream_write(void* stream, const void* ptr, s64 size);
		s64 stream_seek(void* stream, s64 offset);
		s64 stream_length(void* stream);
		s64 stream_position(void* stream);
		void stream_close(void* stream);

		void* gl_context_create();
		void gl_context_make_current(void* context);
		void gl_context_destroy(void* context);

		bool gl_load_glad_loader();
	}
}
