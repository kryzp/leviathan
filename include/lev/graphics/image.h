#pragma once

#include <lev/core/util.h>

namespace Lev
{
	class Image
	{
	public:
		Image();
		Image(const char* path);
		~Image();

		void load(const char* path);
		void free() const;

		const ubyte* data() const;

		int width() const;
		int height() const;
		int nr_channels() const;
		
	private:
		ubyte* m_data = nullptr;

		int m_width = 0;
		int m_height = 0;
		int m_nr_channels = 0;

	};
};
