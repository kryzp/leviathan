#pragma once

#include <lev/core/util.h>

namespace Lev
{
	class Shader
	{
	public:
		Shader();
		Shader(const char* vertex, const char* fragment);

		void use();

		void set(const char* name, bool value) const;
		void set(const char* name, int value) const;
		void set(const char* name, float value) const;

		u32 id;
	};
}
