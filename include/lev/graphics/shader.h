#pragma once

#include <lev/core/util.h>

namespace Lev
{
	class Shader
	{
	public:
		Shader() = default;
		Shader(const char* vertex, const char* fragment);

		void load(const char* vertex, const char* fragment);
		void use();

		void set_bool(const char* name, bool value) const;
		void set_int(const char* name, int value) const;
		void set_float(const char* name, float value) const;

		u32 id;
	};
}
