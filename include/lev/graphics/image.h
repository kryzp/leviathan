#pragma once

#include <lev/core/util.h>

namespace lev::gfx
{
	class Image
	{
	public:
		Image();
		Image(const char* path);
		~Image();

		void load(const char* path);
		void free() const;

		const byte* data() const;

		int width() const;
		int height() const;
		int nr_channels() const;
		
	private:
		byte* m_data = nullptr;

		int m_width = 0;
		int m_height = 0;
		int m_nr_channels = 0;

	};
};
